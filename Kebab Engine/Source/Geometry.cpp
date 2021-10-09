#include "Geometry.h"

#include "Globals.h"

KebabGeometry::~KebabGeometry()
{
	normals.clear();
	texCoords.clear();
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(indices);
}

void KebabGeometry::SetUpBuffers()
{
	vertexArray = new VertexArray();
	vertexBuffer = new VertexBuffer(vertices, sizeof(vertices) * verticesCount);

	vertexArray->AddVertexBuffer(*vertexBuffer);

	indexBuffer = new IndexBuffer(indices, indicesCount);
	vertexArray->SetIndexBuffer(*indexBuffer);
}

void KebabGeometry::Draw()
{
	vertexArray->Bind();
	//indexBuffer->Bind();
	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	//indexBuffer->Unbind();
	vertexArray->Unbind();
}