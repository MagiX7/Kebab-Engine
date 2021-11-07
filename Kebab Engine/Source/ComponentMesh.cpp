#include "Application.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "TextureLoader.h"

#include "imgui/imgui.h"

#include "mmgr/mmgr.h"

#define CHECKERS_HEIGHT 80
#define CHECKERS_WIDTH 80

ComponentMesh::ComponentMesh(GameObject& compOwner)
{
	this->parent = &compOwner;
	this->type = ComponentType::MESH;
	this->active = true;

	drawVertexNormals = false;
	drawTriangleNormals = false;
	
	normalsTriangleSize = normalsVertexSize = 1.0f;
	normalsVertexColor = { 1,0.5f,0 };
	normalsTriangleColor = { 0,0.5f,1.0 };
}

ComponentMesh::~ComponentMesh()
{
	delete(vertexBuffer);
	delete(indexBuffer);
	/*delete(texture);
	delete(checkersTexture);*/

	vertices.clear();
	indices.clear();
	textures.clear();
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
		ImGui::Text("Vertices: %i", vertices.size());
		ImGui::Text("Indices: %i", indices.size());
		ImGui::Text("Textures: %i", textures.size());

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
				//ImGui::NewLine();
				
				ImGui::TreePop();
			}
			//ImGui::ColorPicker3("Normals color", normalColor.ptr(), flags);
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
			//ImGui::ColorPicker3("Normals color", normalColor.ptr(), flags);
		}
	}

	// Temporarily, should move it to material component
	/*if (ImGui::CollapsingHeader("Texture"))
	{
		if(currentTexture && currentTexture == texture)
			ImGui::TextColored({ 255,255,0,255 }, "Path: %s", texture->GetPath().c_str());
		else if (currentTexture == checkersTexture)
			ImGui::TextColored({ 255,255,0,255 }, "Default checkers texture");
		else
			ImGui::TextColored({ 255,255,0,255 }, "Path: This mesh does not have a texture");

		static bool checkers = currentTexture;
		if (currentTexture == checkersTexture) checkers = true;
		else checkers = false;

		if (ImGui::Checkbox("Set checkers", &checkers))
		{
			if (checkers) currentTexture = checkersTexture;
			else if (texture) currentTexture = texture;
			else currentTexture = nullptr;
		}

		ImGui::NewLine();
		ImGui::BulletText("Current Texture: ");
		if (currentTexture)
			ImGui::Image((void*)currentTexture->GetID(), { 150,150 });
		else
		{
			ImGui::SameLine();
			ImGui::Text("No texture.");
		}
	}*/
}

void ComponentMesh::Draw(ComponentMaterial* mat)
{
	BeginDraw(mat);

	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

	// This goes here just to be able to change the normals color while texture is assigned
	mat->Unbind();
	//if (currentTexture) currentTexture->Unbind();

	if (drawVertexNormals)
		DrawVertexNormals();
	if (drawTriangleNormals)
		DrawTriangleNormals();

	EndDraw(mat);
}

void ComponentMesh::SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* tex/*std::vector<Texture> textures*/)
{
	this->vertices = vertices;
	this->indices = indices;

	//if (tex)
	//{
	//	this->texture = tex;
	//	if (!texture)
	//	{
	//		currentTexture = checkersTexture;
	//		//LOG_CONSOLE("Texture is NULL, Gameobject %s. Loaded default checkers texture instead.", parent->GetName().c_str());
	//	}
	//	else currentTexture = texture;
	//}

	SetUpMesh();
}

void ComponentMesh::SetTexture(Texture* tex)
{
	//if (texture) RELEASE(texture);
	/*texture = tex;
	currentTexture = texture;*/
}

void ComponentMesh::DrawVertexNormals()
{
	glBegin(GL_LINES);

	glColor3f(normalsVertexColor.x, normalsVertexColor.y, normalsVertexColor.z);

	for (int i = 0; i < vertices.size(); ++i)
	{
		float3 pos = vertices[i].position;
		glVertex3f(pos.x, pos.y, pos.z);

		if (isKbGeometry)
		{
			float3 normal = vertices[i].normal;
			normal.Normalize();

			glVertex3f(pos.x + normal.x * normalsVertexSize, pos.y, pos.z);

			glVertex3f(pos.x, pos.y, pos.z);
			glVertex3f(pos.x, pos.y + normal.y * normalsVertexSize, pos.z);

			glVertex3f(pos.x, pos.y, pos.z);
			glVertex3f(pos.x, pos.y, pos.z + normal.z * normalsVertexSize);
		}
		else
		{
			float3 n;
			n.x = (vertices[i].position.x + vertices[i].normal.x * normalsVertexSize);
			n.y = (vertices[i].position.y + vertices[i].normal.y * normalsVertexSize);
			n.z = (vertices[i].position.z + vertices[i].normal.z * normalsVertexSize);
			glVertex3f(n.x, n.y, n.z);
		}
	}

	glColor3f(1, 1, 1);

	glEnd();
}

void ComponentMesh::DrawTriangleNormals()
{
	glBegin(GL_LINES);

	glColor3f(normalsTriangleColor.x, normalsTriangleColor.y, normalsTriangleColor.z);

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		float3 v1 = vertices[indices[i]].position;
		float3 v2 = vertices[indices[i + 1]].position;
		float3 v3 = vertices[indices[i + 2]].position;

		float3 tNorm = math::Cross(v2 - v1, v3 - v1);
		tNorm.Normalize();
		//tNorm *= 0.5f;

		float3 tCent;
		tCent.x = (v1.x + v2.x + v3.x) / 3;
		tCent.y = (v1.y + v2.y + v3.y) / 3;
		tCent.z = (v1.z + v2.z + v3.z) / 3;

		glVertex3f(tCent.x, tCent.y, tCent.z);
		glVertex3f(tCent.x + tNorm.x * normalsTriangleSize, tCent.y + tNorm.y * normalsTriangleSize, tCent.z + tNorm.z * normalsTriangleSize);
	}

	glColor3f(1, 1, 1);

	glEnd();
}

JSON_Value* ComponentMesh::Save()
{
	JSON_Value* value = Parser::InitValue();
	JSON_Object* obj = Parser::GetObjectByValue(value);

	Parser::DotSetObjectNumber(obj, "Mesh.vertices", vertices.size());
	Parser::DotSetObjectNumber(obj, "Mesh.indices", indices.size());


	return value;
}

void ComponentMesh::SetUpMesh()
{
	vertexBuffer = new VertexBuffer();
	vertexBuffer->SetData(vertices);

	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	this->parent->AddAABB();

	//texture = TextureLoader::GetInstance()->LoadTexture("Assets/3D Models/bakerHouse.png");

	//if(textures.size() == 0) SetCheckersTexture();
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

	vertexBuffer->Bind();
	indexBuffer->Bind();

	//if (currentTexture) currentTexture->Bind();
	mat->Bind();

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	//glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

}

void ComponentMesh::EndDraw(ComponentMaterial* mat)
{
	glPopMatrix();

	indexBuffer->Unbind();
	vertexBuffer->Unbind();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}