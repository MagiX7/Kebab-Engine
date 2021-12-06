#include "KbPlane.h"

KbPlane::KbPlane(float3 position, float2 size)
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

	SetUpMesh();
}

KbPlane::~KbPlane()
{
}