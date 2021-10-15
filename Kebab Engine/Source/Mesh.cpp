#include "Mesh.h"

KbMesh::KbMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
	: vertices(vertices), indices(indices), textures(textures)
{
	SetUpMesh();
}

void KbMesh::BeginDraw()
{
	vertexBuffer->Bind();
	indexBuffer->Bind();
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
	vertexBuffer->Unbind();
	indexBuffer->Unbind();
}

void KbMesh::SetUpMesh()
{
	//vertexArray = new VertexArray();
	vertexBuffer = new VertexBuffer();
	indexBuffer = new IndexBuffer();

	//vertexBuffer->Bind();
	//indexBuffer->Bind();

	//BufferLayout layout = {
	//	{ShaderDataType::VEC3F, "position"},
	//	//{ShaderDataType::VEC3F, "normal"},
	//};
	//vertexBuffer->SetLayout(layout);

	vertexBuffer->SetData(vertices);
	indexBuffer->SetData(indices.data(), indices.size());

	//vertexArray->AddVertexBuffer(*vertexBuffer);
	//vertexArray->SetIndexBuffer(*indexBuffer);
}