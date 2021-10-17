#include "Mesh.h"

KbMesh::KbMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures, std::vector<float2> texCoords)
	: vertices(vertices), indices(indices), textures(textures)
{
	SetUpMesh();

	/*glGenBuffers(1, &texBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float2), texCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	if (textures.size() == 0)
	{
		texture = new Texture("Assets/3D Models/lenna.png");
	}
}

KbMesh::~KbMesh()
{
	//delete texture;
	glDeleteBuffers(1, &texBuffer);
}

void KbMesh::BeginDraw()
{
	vertexBuffer->Bind();
	indexBuffer->Bind();

	//glBindBuffer(GL_ARRAY_BUFFER, texBuffer);

	if (textures.size() > 0)
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			textures[i].Bind(i);
		}
	}
	else
		texture->Bind();

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	//glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

}

void KbMesh::Draw(bool showNormals)
{
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
}

void KbMesh::EndDraw()
{
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (textures.size() > 0)
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			textures[i].Unbind();
		}
	}
	else texture->Unbind();
	
	indexBuffer->Unbind();
	vertexBuffer->Unbind();
}

void KbMesh::SetUpMesh()
{
	//vertexArray = new VertexArray();
	vertexBuffer = new VertexBuffer();
	vertexBuffer->SetData(vertices);

	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	//vertexBuffer->Bind();
	//indexBuffer->Bind();

	BufferLayout layout = {
		{ShaderDataType::VEC3F, "position"},
		{ShaderDataType::VEC3F, "normal"},
		{ShaderDataType::VEC2F, "texCoords"},
	};
	vertexBuffer->SetLayout(layout);

	//indexBuffer->SetData(indices.data(), indices.size());

	//vertexArray->AddVertexBuffer(*vertexBuffer);
	//vertexArray->SetIndexBuffer(*indexBuffer);
}