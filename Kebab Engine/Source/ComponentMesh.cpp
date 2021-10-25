#include "Application.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "TextureLoader.h"

#include "imgui/imgui.h"

#define CHECKERS_HEIGHT 80
#define CHECKERS_WIDTH 80

ComponentMesh::ComponentMesh(GameObject& compOwner)
{
	this->parent = &compOwner;
	this->type = ComponentType::MESH;
	this->active = true;

	checkersTexture = SetCheckersTexture();
}

ComponentMesh::~ComponentMesh()
{
	RELEASE(vertexBuffer);
	RELEASE(indexBuffer);
	RELEASE(texture);

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

		if (ImGui::Checkbox("Show vertex normals", &app->renderer3D->drawVertexNormals))
		{
		}

		if (ImGui::Checkbox("Show triangles normals", &app->renderer3D->drawTriangleNormals))
		{

		}

	}

	// Temporarily, should move it to material component
	if (ImGui::CollapsingHeader("Texture"))
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
			if (checkers)
				currentTexture = checkersTexture;

			else if (texture)
				currentTexture = texture;

			else currentTexture = nullptr;
		}
	}
}

void ComponentMesh::Draw(bool drawVertexNormals, bool drawTriangleNormals)
{
	BeginDraw();

	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

	if (drawVertexNormals) DrawVertexNormals();
	if (drawTriangleNormals) DrawTriangleNormals();

	EndDraw();
}

void ComponentMesh::SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* tex/*std::vector<Texture> textures*/)
{
	this->vertices = vertices;
	this->indices = indices;

	this->texture = tex;
	if (!texture)
	{
		currentTexture = checkersTexture;
		LOG_CONSOLE("Texture is NULL, Gameobject %s. Loaded default checkers texture instead.", parent->GetName().c_str());
	}
	else currentTexture = texture;

	SetUpMesh();
}

void ComponentMesh::SetTexture(Texture* tex)
{
	if (texture) RELEASE(texture);
	texture = new Texture(*tex);
	currentTexture = texture;
}

void ComponentMesh::DrawVertexNormals()
{
	glBegin(GL_LINES);

	/*float4 currentColor;
	glGetFloatv(GL_CURRENT_COLOR, currentColor.ptr());

	glColor4f(255, 255, 255, 255);*/

	for (int i = 0; i < vertices.size(); ++i)
	{
		float3 pos = vertices[i].position;
		glVertex3f(pos.x, pos.y, pos.z);
		float3 n = vertices[i].position + vertices[i].normal;
		n.Normalize();
		n *= 0.5f;
		glVertex3f(pos.x + n.x, pos.y + n.y, pos.z + n.z);

		/*glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
		float3 n;
		n.x = vertices[i].position.x + vertices[i].normal.x;
		n.y = vertices[i].position.y + vertices[i].normal.y;
		n.z = vertices[i].position.z + vertices[i].normal.z;
		n.Normalize();
		glVertex3f(n.x, n.y, n.z);*/
	}
	
	//glColor4f(currentColor.x, currentColor.y, currentColor.z, currentColor.w);

	glEnd();
}

void ComponentMesh::DrawTriangleNormals()
{
	glBegin(GL_LINES);

	/*float4 currentColor;
	glGetFloatv(GL_CURRENT_COLOR, currentColor.ptr());

	glColor4f(255, 0, 255, 255);*/

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		float3 v1 = vertices[indices[i]].position;
		float3 v2 = vertices[indices[i + 1]].position;
		float3 v3 = vertices[indices[i + 2]].position;

		float3 triangleNormal = math::Cross(v2 - v1, v3 - v1);
		triangleNormal.Normalize();
		triangleNormal *= 0.5f;

		float3 triangleCenter;
		triangleCenter.x = (v1.x + v2.x + v3.x) / 3;
		triangleCenter.y = (v1.y + v2.y + v3.y) / 3;
		triangleCenter.z = (v1.z + v2.z + v3.z) / 3;

		glVertex3f(triangleCenter.x, triangleCenter.y, triangleCenter.z);
		glVertex3f(triangleCenter.x + triangleNormal.x, triangleCenter.y + triangleNormal.y, triangleCenter.z + triangleNormal.z);

		/*float3 vec1 = (vertices[i + 1].position - vertices[i + 2].position);
		float3 vec2 = -(vertices[i].position - vertices[i + 1].position);
		float3 n = math::Cross(vec1, vec2);
		n.Normalize();

		float3 cent;
		cent.x = (vertices[i].position.x + vertices[i + 1].position.x + vertices[i + 2].position.x) / 3;
		cent.y = (vertices[i].position.y + vertices[i + 1].position.y + vertices[i + 2].position.y) / 3;
		cent.z = (vertices[i].position.z + vertices[i + 1].position.z + vertices[i + 2].position.z) / 3;
		glVertex3f(cent.x, cent.y, cent.z);
		glVertex3f(cent.x + n.x, cent.y + n.y, cent.z + n.z);*/
	}

	//glColor4f(currentColor.x, currentColor.y, currentColor.z, currentColor.w);

	glEnd();
}

void ComponentMesh::SetUpMesh()
{
	vertexBuffer = new VertexBuffer();
	vertexBuffer->SetData(vertices);

	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	//texture = TextureLoader::GetInstance()->LoadTexture("Assets/3D Models/bakerHouse.png");

	//if(textures.size() == 0) SetCheckersTexture();
}

void ComponentMesh::BeginDraw()
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

	/*if (textures.size() > 0)
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			textures.data()->Bind(i);
		}
	}
	else */
	//if(texture) texture->Bind();
	if (currentTexture) currentTexture->Bind();

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	//glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

}

void ComponentMesh::EndDraw()
{
	glPopMatrix();
	/*if (textures.size() > 0)
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			textures[i].Unbind();
		}
	}
	else *///texture->Unbind();

	if(currentTexture) currentTexture->Unbind();

	indexBuffer->Unbind();
	vertexBuffer->Unbind();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

Texture* ComponentMesh::SetCheckersTexture()
{
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 225;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)225;
		}
	}
	Texture* ret = new Texture(checkerImage, CHECKERS_WIDTH, CHECKERS_HEIGHT, "Default checkers texture");
	return ret;
}
