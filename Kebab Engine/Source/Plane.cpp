#include "Plane.h"

KbPlane::KbPlane(float3 position, float2 size, GameObject* owner) : KbGeometry(position, owner)
{
	Vertex vertex;

	// Top left
	vertex.position = { position.x, position.y, position.z };
	vertex.normal = vertex.position;
	vertex.texCoords = { 0,1 };
	vertices.push_back(vertex);

	// Bottom left
	vertex.position = { position.x, position.y, position.z + size.y };
	vertex.normal = vertex.position;
	vertex.texCoords = { 0,0 };
	vertices.push_back(vertex);

	// Top right
	vertex.position = { position.x + size.x, position.y, position.z };
	vertex.normal = vertex.position;
	vertex.texCoords = { 1,1 };
	vertices.push_back(vertex);

	// Bottom left
	vertex.position = { position.x + size.x, position.y, position.z + size.y };
	vertex.normal = vertex.position;
	vertex.texCoords = { 1,0 };
	vertices.push_back(vertex);

	uint32_t in[] =
	{
		0,1,2,
		3,2,1
	};
	indices.insert(indices.begin(), in, in + 2 * 3);

	SetUpBuffers();
}

KbPlane::~KbPlane()
{
}

void KbPlane::Draw(bool drawVertexNormals, bool drawTriangleNormals)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//vertexArray->Bind();

	vertexBuffer->Bind();
	indexBuffer->Bind();
	glVertexPointer(3, GL_FLOAT, 0, 0);

	texture->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

	if (drawVertexNormals)
		DrawVertexNormals();
	if (drawTriangleNormals)
		DrawTriangleNormals();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	texture->Unbind();

	vertexBuffer->Unbind();
	indexBuffer->Unbind();

	//vertexArray->Unbind();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}