#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "TextureLoader.h"

#include "imgui/imgui.h"

#define CHECKERS_HEIGHT 50
#define CHECKERS_WIDTH 50

ComponentMesh::ComponentMesh(GameObject& compOwner)
{
	this->parent = &compOwner;
	this->type = ComponentType::MESH;
	this->active = true;
}

ComponentMesh::~ComponentMesh()
{
	RELEASE(vertexBuffer);
	RELEASE(indexBuffer);

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
	}

	// Temporarily, should move it to material component
	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::TextColored({ 255,255,0,255 }, "Path: %s", texture->GetPath().c_str());
	}
}

void ComponentMesh::Draw(bool showNormals)
{
	BeginDraw();

	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

	if (showNormals)
	{
		glBegin(GL_LINES);

		// Draw vertex normals
		for (int i = 0; i < vertices.size(); ++i)
		{
			glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
			float3 n;
			n.x = vertices[i].position.x + vertices[i].normal.x;
			n.y = vertices[i].position.y + vertices[i].normal.y;
			n.z = vertices[i].position.z + vertices[i].normal.z;
			n.Normalize();
			glVertex3f(n.x, n.y, n.z);
		}

		// Draw triangle normals
		for (unsigned int i = 0; i < vertices.size(); i += 3)
		{
			float3 vec1 = (vertices[i + 1].position - vertices[i + 2].position);
			float3 vec2 = -(vertices[i].position - vertices[i + 1].position);
			float3 n = math::Cross(vec1, vec2);
			n.Normalize();

			float3 cent;
			cent.x = (vertices[i].position.x + vertices[i + 1].position.x + vertices[i + 2].position.x) / 3;
			cent.y = (vertices[i].position.y + vertices[i + 1].position.y + vertices[i + 2].position.y) / 3;
			cent.z = (vertices[i].position.z + vertices[i + 1].position.z + vertices[i + 2].position.z) / 3;
			glVertex3f(cent.x, cent.y, cent.z);
			glVertex3f(cent.x + n.x, cent.y + n.y, cent.z + n.z);
		}

		glEnd();
	}

	EndDraw();
}

void ComponentMesh::SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* tex/*std::vector<Texture> textures*/)
{
	this->vertices = vertices;
	this->indices = indices;

	this->texture = tex;
	if (!texture) LOG_CONSOLE("Texture is NULL, gameobject %s", parent->GetName());

	SetUpMesh();
}

void ComponentMesh::SetTexture(Texture* tex)
{
	if (texture) RELEASE(texture);
	texture = new Texture(*tex);
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
	if(texture) texture->Bind();

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	//glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

}

void ComponentMesh::EndDraw()
{
	/*if (textures.size() > 0)
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			textures[i].Unbind();
		}
	}
	else */texture->Unbind();

	indexBuffer->Unbind();
	vertexBuffer->Unbind();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void ComponentMesh::SetCheckersTexture()
{
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	texture = new Texture(checkerImage, CHECKERS_WIDTH, CHECKERS_HEIGHT, "Default checkers texture, generated in code.");
}
