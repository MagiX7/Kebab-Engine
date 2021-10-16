#pragma once

#include "Geometry.h"

class KbCube : public KbGeometry
{
public:
	KbCube(math::float3 pos, math::float3 size) : KbGeometry(pos)
	{
        //vertices.resize(8 * 3); // TRIANGLES
        //indices.resize(12 * 3); // 4 Vertex for each face

        vertices = new float3[8 * 3];
        verticesCount = 8 * 3;
        //float3 v[]=
        //{
        //    {pos.x, pos.y, pos.z },          // Top Left
        //    {pos.x, pos.y + size.y,pos.z},  // Bottom left
        //    {pos.x + size.x, pos.y,pos.z},  // Top Right
        //    {pos.x + size.x, pos.y + size.y, pos.z},  // Bottom right

        //    {pos.x + size.x, pos.y,pos.z + size.z},  // Top Back Right
        //    {pos.x + size.x, pos.y + size.y,pos.z + size.z},   // Bottom Back right

        //    {pos.x, pos.y, pos.z + size.z},          // Top back Left
        //    {pos.x, pos.y + size.y,pos.z + size.z}  // Bottom back left
        //};
        float3 v[] =
        {
            {pos.x + size.x, pos.y,pos.z},  // Top Right
            {pos.x, pos.y, pos.z },         // Top Left
            {pos.x, pos.y + size.y,pos.z},  // Bottom left
            {pos.x + size.x, pos.y + size.y, pos.z},  // Bottom right
            {pos.x + size.x, pos.y + size.y,pos.z + size.z},   // Bottom Back right
            {pos.x + size.x, pos.y,pos.z + size.z},  // Top Back Right
            {pos.x, pos.y, pos.z + size.z},          // Top back Left
            {pos.x, pos.y + size.y,pos.z + size.z}  // Bottom back left
        };
        std::copy(v, v + verticesCount, vertices);

        indices = new uint32_t[12 * 3];
        indicesCount = 12 * 3;
        uint32_t in[] =
        {
            // Front
            0,1,2,
            2,3,0,

            // Right
            5,0,3,
            3,4,5,

            // Back
            5,6,7,
            7,4,5,

            // Left
            6,1,2,
            2,7,6,

            // Top
            5,6,1,
            1,0,5,

            // Bottom
            4,7,2,
            2,3,4,
        };
        std::copy(in, in + indicesCount, indices);

        SetUpBuffers();
        //vertexArray = new VertexArray();
        //vertexBuffer = new VertexBuffer(vertices.data(), sizeof(vertices.data()[0]) * vertices.size());

        //vertexArray->AddVertexBuffer(*vertexBuffer);

        ////indexBuffer = new IndexBuffer(primitive->GetIndices().data() , sizeof(primitive->GetIndices().data()) / sizeof(uint32_t));
        //indexBuffer = new IndexBuffer(indices.data(), sizeof(indices.data()[0]) * indices.size());
        //vertexArray->SetIndexBuffer(*indexBuffer);
	}

    ~KbCube()
    {
        RELEASE_ARRAY(vertices);
        RELEASE_ARRAY(indices);
        RELEASE_ARRAY(normals);
    }
};