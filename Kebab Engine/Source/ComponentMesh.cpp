#include "Application.h"

#include "Editor.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "PanelAssets.h"

#include "ResourceManager.h"

#include "Model.h"

#include "TextureLoader.h"

#include "imgui/imgui.h"

#include "mmgr/mmgr.h"

#define CHECKERS_HEIGHT 80
#define CHECKERS_WIDTH 80

ComponentMesh::ComponentMesh(GameObject* compOwner, const std::string& meshPath)
{
	this->parent = compOwner;
	this->type = ComponentType::MESH;
	this->active = true;

	drawVertexNormals = false;
	drawTriangleNormals = false;
	
	normalsTriangleSize = normalsVertexSize = 1.0f;
	normalsVertexColor = { 1,0.5f,0 };
	normalsTriangleColor = { 0,0.5f,1.0 };

	mesh = new KbMesh(meshPath);
}

ComponentMesh::~ComponentMesh()
{
	/*if (mesh)
	{
		delete mesh;
		mesh = nullptr;
	}*/
}

void ComponentMesh::Enable()
{
	this->active = true;
}

void ComponentMesh::Disable()
{
	this->active = false;
}

void ComponentMesh::Update()
{
}

void ComponentMesh::DrawOnInspector()
{
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Vertices: %i", mesh->GetVertices().size());
		ImGui::Text("Indices: %i", mesh->GetIndices().size());
		if (model)
		{
			// Need to do this because the meshes are not shared ptr, and i need to find how many meshes there are in the mode
			// and their usage by this way. I took care that the meshes are nut duplicated in the same model.
			int refCount = model.use_count() / (model->GetMeshes().size()) - 1; // -1 because of the model in the resources map
			ImGui::BulletText("Reference count: %i", refCount); 
		}

		ImGui::Checkbox("Show vertex normals", &drawVertexNormals);
		if (drawVertexNormals)
		{
			if (ImGui::TreeNodeEx("Attributes"))
			{
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::DragFloat("Size", &normalsVertexSize, 0.01f);


				ImGuiColorEditFlags flags = ImGuiColorEditFlags_PickerHueBar;
				ImGui::ColorEdit3("Color", normalsVertexColor.ptr());
				
				ImGui::NewLine();
				ImGui::Separator();
				
				ImGui::TreePop();
			}
		}

		ImGui::Checkbox("Show triangles normals", &drawTriangleNormals);

		if (drawTriangleNormals)
		{
			if(ImGui::TreeNodeEx("Attributes "))
			{
				ImGui::Separator();
				ImGui::NewLine();

				ImGui::DragFloat("Size ", &normalsTriangleSize, 0.01f);

				ImGui::ColorEdit3("Color ", normalsTriangleColor.ptr());

				ImGui::NewLine();
				ImGui::Separator();

				ImGui::TreePop();
			}
		}

		if (ImGui::Button("Change Mesh"))
			menuSelectMesh = !menuSelectMesh;

		if (menuSelectMesh)
			ShowMeshesMenu();
	}
}

void ComponentMesh::ShowMeshesMenu()
{
	//ImGui::SetNextWindowPos(ImVec2(2, 2));

	ImGui::Begin("Select Mesh", &menuSelectMesh);

	std::vector<KbMesh*> meshes = app->editor->assetsPanel->meshes;

	ImGui::Columns(2, 0, false);

	for (std::vector<KbMesh*>::const_iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		std::string name = (*it)->GetName();

		/*std::string nameShow = name.substr(0, name.find_last_of("_"));
		nameShow = nameShow.substr(0, nameShow.find_last_of("_"));*/
		std::string nameShow = name.substr(0, name.find_last_of("."));

		ImGui::ImageButton((ImTextureID)app->editor->assetsPanel->meshTex->GetID(), { 100,100 });
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			SetMesh((*it));
			SetMeshPath((*it)->GetLibraryPath());
		}

		ImGui::Text(nameShow.c_str());

		ImGui::NextColumn();
	}

	if (!ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left || ImGuiMouseButton_Right))
		menuSelectMesh = false;

	ImGui::End();
}

void ComponentMesh::Draw(ComponentMaterial* mat, ComponentCamera* cam)
{
	//BeginDraw(mat);
	mat->Bind(cam);
	mesh->GetVertexArray()->Bind();
	glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

	mat->Unbind();

	if (drawVertexNormals)
		mesh->DrawVertexNormals(normalsVertexSize, normalsVertexColor);
	if (drawTriangleNormals)
		mesh->DrawTriangleNormals(normalsTriangleSize, normalsTriangleColor);
	
	mesh->GetVertexArray()->Unbind();

	//EndDraw(mat);
}

void ComponentMesh::SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* tex)
{
	mesh->SetData(vertices, indices);

	AABB aabb;

	aabb.SetNegativeInfinity();
	int size = vertices.size();

	for (int i = 0; i < size; ++i)
		aabb.Enclose(vertices[i].position);

	parent->SetGlobalAABB(aabb);
}

void ComponentMesh::SetMeshPath(const std::string& path)
{
	mesh->SetPath(path);
}

void ComponentMesh::SetMesh(KbMesh* newMesh)
{
	//delete mesh; 
	mesh = nullptr;
	mesh = newMesh;

	AABB aabb;

	aabb.SetNegativeInfinity();
	int size = mesh->vertices.size();

	for (int i = 0; i < size; ++i)
		aabb.Enclose(mesh->vertices[i].position);

	parent->SetGlobalAABB(aabb);

}

void ComponentMesh::SetModel(std::shared_ptr<KbModel> m)
{
	model = m;
}

JSON_Value* ComponentMesh::Save()
{
	JSON_Value* value = Parser::InitValue();
	JSON_Object* obj = Parser::GetObjectByValue(value);

	json_object_set_number(obj, "Type", 1);

	Parser::DotSetObjectString(obj, "model library path", model->GetLibraryPath().c_str());
	Parser::DotSetObjectNumber(obj, "model uuid", model->uuid);
	Parser::DotSetObjectNumber(obj, "mesh uuid", mesh->uuid);
	Parser::DotSetObjectString(obj, "mesh library path", mesh->GetLibraryPath().c_str());
	Parser::DotSetObjectString(obj, "mesh name", mesh->GetName().c_str());


	return value;
}

void ComponentMesh::Load(JSON_Object* obj, GameObject* parent)
{
	std::string modelPath = json_object_get_string(obj, "model library path");

	int uuid = json_object_get_number(obj, "mesh uuid");
	std::string path = json_object_get_string(obj, "mesh library path");
	/*std::string meshName = json_object_get_string(obj, "mesh name");
	meshName += "__" + std::to_string(uuid);*/
	if (model = std::static_pointer_cast<KbModel>(ResourceManager::GetInstance()->IsAlreadyLoaded(modelPath)))
	{
		for (int i = 0; i < model->GetMeshes().size(); ++i)
		{
			if (model->GetMeshes()[i]->GetLibraryPath() == path)
			{
				mesh = model->GetMeshes()[i];
				mesh->uuid = uuid;
			}
		}

		/*static int it = 0;
		if (it >= model->GetMeshes().size())
			it = 0;
		mesh = model->GetMeshes()[it++];
		mesh->uuid = uuid;*/
	}


	AABB aabb;

	aabb.SetNegativeInfinity();
	int size = mesh->vertices.size();

	for (int i = 0; i < size; ++i)
		aabb.Enclose(mesh->vertices[i].position);

	parent->SetGlobalAABB(aabb);
}

void ComponentMesh::BeginDraw(ComponentMaterial* mat)
{
	/*ComponentTransform* t = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
	if (t)
	{
		float4x4 mat = t->GetGlobalMatrix();
		glPushMatrix();
		glMultMatrixf(mat.Transposed().ptr());
	}*/
	
	mat->Bind(nullptr);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	mesh->BeginDraw();
	//mat->Bind();

	//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	//glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

}

void ComponentMesh::EndDraw(ComponentMaterial* mat)
{
	//glPopMatrix();
	//mat->Unbind();

	mesh->EndDraw();

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}