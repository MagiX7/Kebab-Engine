#include "Application.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "MeshLoader.h"
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
	/*int start = meshPath.find_last_of('\\');
	if (start == 0)
		start = meshPath.find_last_of('/');

	mesh->SetName(meshPath.substr(start + 1));
	mesh->SetPath(meshPath);*/
}

ComponentMesh::~ComponentMesh()
{
	delete mesh;
	mesh = nullptr;
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
		//ImGui::Text("Textures: %i", textures.size());

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
	}
}

void ComponentMesh::Draw(ComponentMaterial* mat)
{
	BeginDraw(mat);

	glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

	// This goes here just to be able to change the normals color while texture is assigned
	mat->Unbind();
	//if (currentTexture) currentTexture->Unbind();

	if (drawVertexNormals)
		mesh->DrawVertexNormals(normalsVertexSize, normalsVertexColor);
	if (drawTriangleNormals)
		mesh->DrawTriangleNormals(normalsTriangleSize, normalsTriangleColor);

	EndDraw(mat);
}

void ComponentMesh::SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* tex/*std::vector<Texture> textures*/)
{
	mesh->SetData(vertices, indices);

	AABB aabb;

	aabb.SetNegativeInfinity();
	int size = vertices.size();
	//aabb.Enclose(&vertices.data()->position, size);
	for (int i = 0; i < size; ++i)
		aabb.Enclose(vertices[i].position);

	parent->SetGlobalAABB(aabb);

	////// Now done in SetGlobalAABB //////
	/*ComponentTransform* tr = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
	parent->UpdateAABB(tr->GetLocalMatrix());*/
}

void ComponentMesh::SetMeshPath(const std::string& path)
{
	mesh->SetPath(path);
}

JSON_Value* ComponentMesh::Save()
{
	JSON_Value* value = Parser::InitValue();
	JSON_Object* obj = Parser::GetObjectByValue(value);

	json_object_set_number(obj, "Type", 1);

	/*Parser::DotSetObjectNumber(obj, "vertices", mesh->GetVertices().size());
	Parser::DotSetObjectNumber(obj, "indices", mesh->GetIndices().size());*/
	Parser::DotSetObjectString(obj, "mesh path", mesh->GetPath().c_str());
	Parser::DotSetObjectString(obj, "mesh name", mesh->GetName().c_str());


	return value;
}

void ComponentMesh::Load(JSON_Object* obj, GameObject* parent)
{
	/*std::vector<Vertex> vertices;
	vertices.resize(json_object_get_number(obj, "vertices"));
	std::vector<uint32_t> indices;
	indices.resize(json_object_get_number(obj, "indices"));*/

	std::string path = json_object_get_string(obj, "mesh path");
	std::string meshName = json_object_get_string(obj, "mesh name");

	mesh = MeshLoader::GetInstance()->LoadMeshCustomFormat(meshName.c_str(), parent);

	AABB aabb;

	aabb.SetNegativeInfinity();
	int size = mesh->vertices.size();
	//aabb.Enclose(&vertices.data()->position, size);
	for (int i = 0; i < size; ++i)
		aabb.Enclose(mesh->vertices[i].position);

	parent->SetGlobalAABB(aabb);
	//this->parent = parent;
}

void ComponentMesh::BeginDraw(ComponentMaterial* mat)
{
	ComponentTransform* t = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
	if (t)
	{
		float4x4 mat = t->GetLocalMatrix();
		glPushMatrix();
		glMultMatrixf(mat.Transposed().ptr());
	}
	

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	mesh->BeginDraw();
	mat->Bind();

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	//glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

}

void ComponentMesh::EndDraw(ComponentMaterial* mat)
{
	glPopMatrix();

	mesh->EndDraw();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}