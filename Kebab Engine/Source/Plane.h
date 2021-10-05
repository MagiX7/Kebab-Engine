#pragma once

#include "Geometry.h"

class KebabPlane : public KebabGeometry
{
public:
	// Position is the top left corner.
	KebabPlane(float3 position, float2 size) : KebabGeometry(position)
	{
		vertices.resize(4);
		indices.resize(6);

		vertices =
		{
			position.x, position.y, position.z,
			position.x, position.y, position.z + size.y,
			position.x + size.x, position.y, position.z,
			position.x+size.x, position.y, position.z + size.y
		};
		
		indices =
		{
			0,1,2,
			3,2,1
		};
	}

	virtual ~KebabPlane()
	{
		vertices.clear();
		normals.clear();
		texCoords.clear();
		indices.clear();
	}
};