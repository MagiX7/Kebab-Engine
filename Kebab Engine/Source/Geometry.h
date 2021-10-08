#pragma once

#include "VertexArray.h"

#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/float2.h"

#include <vector>

class KebabGeometry
{
public:
	KebabGeometry()
	{
		vertexArray = new VertexArray();
		vertexBuffer = new VertexBuffer();
		indexBuffer = new IndexBuffer();
	}
	KebabGeometry(math::float3 pos) : position(pos) {}
	virtual ~KebabGeometry()
	{
		vertices.clear();
		normals.clear();
		texCoords.clear();
		indices.clear();
	};

	inline void SetPos(const math::float3& pos) { position = pos; }
	inline const math::float3& GetPosition() { return position; }

	inline const std::vector<float>& GetVertices() { return vertices; }
	inline const std::vector<float>& GetNormals() { return normals; }
	inline const std::vector<float>& GetTextureCoords() { return texCoords; }
	inline const std::vector<uint32_t>& GetIndices() { return indices; }

	void SetUpBuffers()
	{
		vertexArray = new VertexArray();
		vertexBuffer = new VertexBuffer(vertices.data(), sizeof(vertices.data()[0]) * vertices.size());

		/*BufferLayout layout =
		{
			{ShaderDataType::VEC3F, "position"}
		};

		vertexBuffer->SetLayout(layout);*/
		vertexArray->AddVertexBuffer(*vertexBuffer);

		//indexBuffer = new IndexBuffer(primitive->GetIndices().data() , sizeof(primitive->GetIndices().data()) / sizeof(uint32_t));
		indexBuffer = new IndexBuffer(indices.data(), sizeof(indices.data()[0]) * indices.size());
		vertexArray->SetIndexBuffer(*indexBuffer);
	}

	void Draw()
	{
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		vertexArray->Unbind();
	}

public:
	VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;


	std::vector<float>vertices;
	std::vector<float>normals;
	std::vector<float>texCoords;
	std::vector<uint32_t>indices;

	math::float3 position;
};