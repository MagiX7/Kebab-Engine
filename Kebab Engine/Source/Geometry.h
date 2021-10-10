#pragma once

#include "VertexArray.h"

#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/float2.h"

#include <vector>

#include <iostream>

class KebabGeometry
{
public:
	KebabGeometry() {};
	KebabGeometry(math::float3 pos) : position(pos) {}
	
	virtual ~KebabGeometry();

	inline void SetPos(const math::float3& pos) { position = pos; }
	inline const math::float3& GetPosition() { return position; }

	inline const float3* GetVertices() { return vertices; }
	inline const float3* GetNormals() { return normals; }
	inline const std::vector<float>& GetTextureCoords() { return texCoords; }
	inline const uint32_t* GetIndices() { return indices; }

	void SetUpBuffers();

	void Draw();

public:
	//VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	float3* vertices;
	uint32_t verticesCount = 0;
	uint32_t* indices;
	uint32_t indicesCount = 0;

	float3* normals;
	uint32_t normalsCount;
	//std::vector<float>normals;
	std::vector<float>texCoords;

	math::float3 position;
};