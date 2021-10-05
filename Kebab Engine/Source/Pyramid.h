#pragma once

#include "Geometry.h"

class Pyramid : public Geometry
{
public:
	// Position is the middle of the base
	Pyramid(float3 pos, float height, float baseWidth) : Geometry(pos)
	{
		float ap = sqrt(pow(height, 2) + pow(baseWidth / 2, 2));

		float distToCorner = sqrt(pow(baseWidth / 2, 2) + pow(baseWidth / 2, 2));

		float3 frontRightCorner = { pos.x - distToCorner, pos.y, pos.z + distToCorner };
		float3 backRightCorner = { pos.x - distToCorner, pos.y, pos.z - distToCorner };
		float3 frontLeftCorner = { pos.x + distToCorner, pos.y, pos.z + distToCorner };
		float3 backLeftCorner = { pos.x + distToCorner, pos.y, pos.z - distToCorner };

		vertices.resize(5 * 3); // Tris
		indices.resize(6 * 3);
		float v[] =
		{
			pos.x, pos.y + height,pos.z, // Up
			frontLeftCorner.x, frontLeftCorner.y, frontLeftCorner.z, // Bottom left
			frontRightCorner.x, frontRightCorner.y, frontRightCorner.z, // Bottom right
			backRightCorner.x, backRightCorner.y, backRightCorner.z, // Bottom back right
			backLeftCorner.x, backLeftCorner.y, backLeftCorner.z, // Bottom back left
		};
		
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

		for (int i = 0; i < vertices.size(); ++i)
			vertices[i] = v[i];

		for (int i = 0; i < indices.size(); ++i)
			indices[i] = in[i];
		
	}

	virtual ~Pyramid()
	{
		vertices.clear();
		normals.clear();
		texCoords.clear();
		indices.clear();
	}
};