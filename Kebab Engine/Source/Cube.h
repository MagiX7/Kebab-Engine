#pragma once

#include "Geometry.h"

class KebabCube : public KebabGeometry
{
public:
	KebabCube(math::float3 pos, math::float3 size) : KebabGeometry(pos)
	{
        vertices.resize(8 * 3); // TRIANGLES
        indices.resize(12 * 3); // 4 Vertex for each face

        vertices =
        {
            pos.x, pos.y, pos.z,          // Top Left
            pos.x, pos.y + size.y,pos.z,  // Bottom left
            pos.x + size.x, pos.y,pos.z,  // Top Right
            pos.x + size.x, pos.y + size.y, pos.z,  // Bottom right

            pos.x + size.x, pos.y,pos.z + size.z,  // Top Back Right
            pos.x + size.x, pos.y + size.y,pos.z + size.z,   // Bottom Back right

            pos.x, pos.y, pos.z + size.z,          // Top back Left
            pos.x, pos.y + size.y,pos.z + size.z,  // Bottom back left
        };

        indices =
        {
            // Front
            0,1,2,
            3,2,1,

            // Right
            2,3,4,
            5,4,3,

            // Back
            4,5,6,
            7,6,5,

            // Left
            6,7,0,
            1,0,7,

            // Top
            0,4,6,
            2,4,0,

            // Bottom
            1,5,7,
            3,5,1
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

    virtual ~KebabCube()
    {
        vertices.clear();
        normals.clear();
        texCoords.clear();
        indices.clear();
    }
};