#pragma once

#include "VertexArray.h"
#include "Texture.h"

#include "ComponentMesh.h"

#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/float2.h"

#include <vector>

#include <iostream>

class KbGeometry : public ComponentMesh
{
public:
	KbGeometry(GameObject* parent) : ComponentMesh(parent) {};
	KbGeometry(math::float3 pos, GameObject* parent) : position(pos), ComponentMesh(parent) {}
	
	virtual ~KbGeometry();

	inline void SetPos(const math::float3& pos) { position = pos; }
	inline const math::float3& GetPosition() { return position; }

	//inline const float3* GetVertices() { return vertices; }
	inline const float3* GetNormals() { return normals; }
	//inline const std::vector<float>& GetTextureCoords() { return texCoords; }
	//inline const float2* GetTexCoords() { return texCoords; }
	//inline const uint32_t* GetIndices() { return indices; }

	void SetUpBuffers();

	//virtual void Draw();

	//void SetCheckerTexture();

public:
	//VertexArray* vertexArray;
	/*VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Texture* texture;*/

	/*float3* vertices;
	uint32_t verticesCount = 0;

	uint32_t* indices;
	uint32_t indicesCount = 0;*/

	float3* normals;
	uint32_t normalsCount = 0;
	
	/*float2* texCoords;
	uint32_t texCoordsCount = 0;*/

	math::float3 position;

	unsigned int texBuffer;

private:
	unsigned int textureID;
};