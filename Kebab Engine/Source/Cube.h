#pragma once

#include "Geometry.h"

class KbCube : public KbGeometry
{
public:
	KbCube(math::float3 pos, math::float3 size) : KbGeometry(pos)
	{
        //vertices = new float3[8 * 3];
        //verticesCount = 8 * 3;
        //
        //float3 v[] =
        //{
        //    {pos.x, pos.y, pos.z },         // Top Left
        //    {pos.x, pos.y + size.y,pos.z},  // Bottom left
        //    {pos.x + size.x, pos.y + size.y, pos.z},  // Bottom right
        //    {pos.x + size.x, pos.y,pos.z},  // Top Right
        //    {pos.x + size.x, pos.y + size.y,pos.z + size.z},   // Bottom Back right
        //    {pos.x + size.x, pos.y,pos.z + size.z},  // Top Back Right
        //    {pos.x, pos.y + size.y,pos.z + size.z},  // Bottom back left
        //    {pos.x, pos.y, pos.z + size.z}          // Top back Left
        //};
        //std::copy(v, v + verticesCount, vertices);

        //indices = new uint32_t[12 * 3];
        //indicesCount = 12 * 3;
        //uint32_t in[] =
        //{
        //    // Front
        //    0,1,2,
        //    2,3,0,

        //    // Right
        //    5,0,3,
        //    3,4,5,

        //    // Back
        //    5,6,7,
        //    7,4,5,

        //    // Left
        //    6,1,2,
        //    2,7,6,

        //    // Top
        //    5,6,1,
        //    1,0,5,

        //    // Bottom
        //    4,7,2,
        //    2,3,4,
        //};
        //std::copy(in, in + indicesCount, indices);

        vertices = new float3[8 * 3];
        verticesCount = 8 * 3;
        float3 v[] =
        {
            {-0.5f,  0.5f,  0.0f},
            {-0.5f, -0.5f,  0.0f },
            { 0.5f, -0.5f,  0.0f },
            { 0.5f,  0.5f,  0.0f },
            { 0.5f, -0.5f, -1.0f },
            { 0.5f,  0.5f, -1.0f },
            {-0.5f, -0.5f, -1.0f },
            {-0.5f,  0.5f, -1.0f },

        };

        std::copy(v, v + verticesCount, vertices);

        indices = new uint32_t[12 * 3];
        indicesCount = 12 * 3;
        uint32_t in[] =
        {
           0,1,2,
           2,3,0,

           3,2,4,
           4,5,3,

           5,4,6,
           6,7,5,
           
           7,6,1,
           1,0,7,

           3,5,7,
           7,0,3,

           1,6,4,
           4,2,1
           
        };
        std::copy(in, in + indicesCount, indices);


        texCoordsCount = 8;
        texCoords = new float2[texCoordsCount];
        /*float2 tc[] =
        {
            {1,1},
            {1,0},
            {0,0},
            {0,1},

            {0,0},
            {1,0},
            {1,1},
            {0,1},
        };*/

        float2 tc[] =
        {
            {0,1},
            {0,0},
            {1,0},
            {1,1},

            {0,1},
            {0,0},
            {1,0},
            {1,1}
        };

        std::copy(tc, tc + texCoordsCount, texCoords);

        SetUpBuffers();
	}

    ~KbCube()
    {
        RELEASE_ARRAY(vertices);
        RELEASE_ARRAY(indices);
        RELEASE_ARRAY(normals);
    }
};