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
	RELEASE(texture);
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
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	vertexArray->Bind();
	texture->Bind();
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
	
	texture->Unbind();
	vertexArray->Unbind();
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}