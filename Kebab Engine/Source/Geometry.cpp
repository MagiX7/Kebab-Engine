#include "Geometry.h"

#include "Globals.h"

KebabGeometry::~KebabGeometry()
{
	//normals.clear();
	texCoords.clear();
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(normals);

	RELEASE(vertexBuffer);
	RELEASE(indexBuffer);
}

void KebabGeometry::SetUpBuffers()
{
	vertexBuffer = new VertexBuffer(vertices, sizeof(vertices) * verticesCount);
	indexBuffer = new IndexBuffer(indices, indicesCount);
}

void KebabGeometry::Draw()
{
	/*vertexArray->Bind();
	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	vertexArray->Unbind();*/
}