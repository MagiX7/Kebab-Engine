#include "Geometry.h"

#include "Globals.h"

KbGeometry::~KbGeometry()
{
	//normals.clear();
	texCoords.clear();
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(normals);

	RELEASE(vertexBuffer);
	RELEASE(indexBuffer);
	RELEASE(vertexArray);
}

void KbGeometry::SetUpBuffers()
{
	vertexBuffer = new VertexBuffer(vertices, sizeof(vertices) * verticesCount);
	indexBuffer = new IndexBuffer(indices, indicesCount);
	vertexArray = new VertexArray();
	vertexArray->AddVertexBuffer(*vertexBuffer);
	vertexArray->SetIndexBuffer(*indexBuffer);
}

void KbGeometry::Draw(bool showNormals)
{
	vertexArray->Bind();
	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	vertexArray->Unbind();
}