#include "KbPyramid.h"

KbPyramid::KbPyramid(float3 pos, float height, float baseWidth)
{
	float ap = sqrt(pow(height, 2) + pow(baseWidth / 2, 2));

	float distToCorner = sqrt(pow(baseWidth / 2, 2) + pow(baseWidth / 2, 2));

	float3 frontRightCorner = { pos.x - distToCorner, pos.y, pos.z + distToCorner };
	float3 backRightCorner = { pos.x - distToCorner, pos.y, pos.z - distToCorner };
	float3 frontLeftCorner = { pos.x + distToCorner, pos.y, pos.z + distToCorner };
	float3 backLeftCorner = { pos.x + distToCorner, pos.y, pos.z - distToCorner };

	Vertex vertex;
	vertex.position = { pos.x, pos.y + height,pos.z };
	vertex.texCoords = { 0.5,1 };
	vertex.normal = vertex.position;
	vertices.push_back(vertex);

	vertex.position = { frontLeftCorner.x, frontLeftCorner.y, frontLeftCorner.z };
	vertex.texCoords = { 1, 0 };
	vertex.normal = vertex.position;
	vertices.push_back(vertex);

	vertex.position = { frontRightCorner.x, frontRightCorner.y, frontRightCorner.z };
	vertex.texCoords = { 0, 0 };
	vertex.normal = vertex.position;
	vertices.push_back(vertex);

	vertex.position = { backRightCorner.x, backRightCorner.y, backRightCorner.z };
	vertex.texCoords = { 1, 0 };
	vertex.normal = vertex.position;
	vertices.push_back(vertex);

	vertex.position = { backLeftCorner.x, backLeftCorner.y, backLeftCorner.z };
	vertex.texCoords = { 0, 0 };
	vertex.normal = vertex.position;
	vertices.push_back(vertex);

	uint32_t in[] =
	{
		// Front
		0,1,2,

		// Right
		0,2,3,

		// Back
		0,3,4,

		// Left
		0,4,1,

		// Bottom
		1,3,4,
		2,3,1
	};
	indices.insert(indices.begin(), in, in + 6 * 3);

	SetUpMesh();
}

KbPyramid::~KbPyramid()
{
}
