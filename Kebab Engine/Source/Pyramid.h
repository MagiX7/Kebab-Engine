#pragma once

#include "Geometry.h"

class KebabPyramid : public KebabGeometry
{
public:
	// Position is the middle of the base
	KebabPyramid(float3 pos, float height, float baseWidth) : KebabGeometry(pos)
	{
		float ap = sqrt(pow(height, 2) + pow(baseWidth / 2, 2));

		float distToCorner = sqrt(pow(baseWidth / 2, 2) + pow(baseWidth / 2, 2));

		float3 frontRightCorner = { pos.x - distToCorner, pos.y, pos.z + distToCorner };
		float3 backRightCorner = { pos.x - distToCorner, pos.y, pos.z - distToCorner };
		float3 frontLeftCorner = { pos.x + distToCorner, pos.y, pos.z + distToCorner };
		float3 backLeftCorner = { pos.x + distToCorner, pos.y, pos.z - distToCorner };

		vertices.resize(5 * 3); // Tris
		indices.resize(6 * 3);
		vertices =
		{
			pos.x, pos.y + height,pos.z, // Up
			frontLeftCorner.x, frontLeftCorner.y, frontLeftCorner.z, // Bottom left
			frontRightCorner.x, frontRightCorner.y, frontRightCorner.z, // Bottom right
			backRightCorner.x, backRightCorner.y, backRightCorner.z, // Bottom back right
			backLeftCorner.x, backLeftCorner.y, backLeftCorner.z, // Bottom back left
		};
		
		indices =
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

		vertexArray = new VertexArray();
		vertexBuffer = new VertexBuffer(vertices.data(), sizeof(vertices.data()[0]) * vertices.size());

		BufferLayout layout =
		{
			{ShaderDataType::VEC3F, "position"}
		};

		vertexBuffer->SetLayout(layout);
		vertexArray->AddVertexBuffer(*vertexBuffer);

		//indexBuffer = new IndexBuffer(primitive->GetIndices().data() , sizeof(primitive->GetIndices().data()) / sizeof(uint32_t));
		indexBuffer = new IndexBuffer(indices.data(), sizeof(indices.data()[0]) * indices.size());
		vertexArray->SetIndexBuffer(*indexBuffer);
	}

	virtual ~KebabPyramid()
	{
		vertices.clear();
		normals.clear();
		texCoords.clear();
		indices.clear();
	}
};