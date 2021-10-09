#pragma once

#include "Geometry.h"

class KebabPlane : public KebabGeometry
{
public:
	// Position is the top left corner.
	KebabPlane(float3 position, float2 size) : KebabGeometry(position)
	{
		/*vertices.resize(4);
		indices.resize(6);

		vertices =
		{
			{position.x, position.y, position.z},
			{position.x, position.y, position.z + size.y},
			{position.x + size.x, position.y, position.z},
			{position.x + size.x, position.y, position.z + size.y}
		};
		*/
		/*indices =
		{
			0,1,2,
			3,2,1
		};*/

		vertexArray = new VertexArray();
		//vertexBuffer = new VertexBuffer(vertices.data(), sizeof(vertices.data()[0]) * vertices.size());

		BufferLayout layout =
		{
			{ShaderDataType::VEC3F, "position"}
		};

		vertexBuffer->SetLayout(layout);
		vertexArray->AddVertexBuffer(*vertexBuffer);

		//indexBuffer = new IndexBuffer(primitive->GetIndices().data() , sizeof(primitive->GetIndices().data()) / sizeof(uint32_t));
		//indexBuffer = new IndexBuffer(indices.data(), sizeof(indices.data()[0]) * indices.size());
		vertexArray->SetIndexBuffer(*indexBuffer);

	}
};