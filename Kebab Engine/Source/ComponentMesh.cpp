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

ComponentMesh::ComponentMesh(GameObject& compOwner, const std::string& meshPath) : meshPath(meshPath)
{
	this->parent = &compOwner;
	this->type = ComponentType::MESH;
	this->active = true;

	drawVertexNormals = false;
	drawTriangleNormals = false;
	
	normalsTriangleSize = normalsVertexSize = 1.0f;
	normalsVertexColor = { 1,0.5f,0 };
	normalsTriangleColor = { 0,0.5f,1.0 };

	int start = meshPath.find_last_of('\\');
	if (start == 0)
		start = meshPath.find_last_of('/');

	meshName = meshPath.substr(start + 1);
}

ComponentMesh::~ComponentMesh()
{
	delete(vertexBuffer);
	delete(indexBuffer);

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

	SetUpMesh();
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

	json_object_set_number(obj, "Type", 1);

	Parser::DotSetObjectNumber(obj, "vertices", vertices.size());
	Parser::DotSetObjectNumber(obj, "indices", indices.size());
	Parser::DotSetObjectString(obj, "path", meshPath.c_str());

	return value;
}

void ComponentMesh::Load(JSON_Object* obj, GameObject* parent)
{
	vertices.resize(json_object_get_number(obj, "vertices"));
	indices.resize(json_object_get_number(obj, "indices"));

	std::string path = json_object_dotget_string(obj, "path");
	MeshLoader::GetInstance()->LoadMeshCustomFormat(meshName.c_str(), parent);
}

void ComponentMesh::SetUpMesh()
{
	vertexBuffer = new VertexBuffer();
	vertexBuffer->SetData(vertices);

	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	this->parent->AddAABB();
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