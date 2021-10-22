#include "Cube.h"

#include "Globals.h"

KbCube::KbCube(math::float3 pos, math::float3 size, GameObject* owner) : KbGeometry(owner)
{
    //vertex.position = new float3[8 * 3];
    //verticesCount = 8 * 3;
    //float3 v[] =
    Vertex vertex;

    vertex.position = { -0.5f,  0.5f,  0.0f };
    vertex.texCoords = { 0,1 };
    vertices.push_back(vertex);

    vertex.position = { -0.5f, -0.5f,  0.0f };
    vertex.texCoords = { 0,0 };
    vertices.push_back(vertex);

    vertex.position = { 0.5f, -0.5f,  0.0f };
    vertex.texCoords = { 1,0 };
    vertices.push_back(vertex);

    vertex.position = { 0.5f,  0.5f,  0.0f };
    vertex.texCoords = { 1,1 };
    vertices.push_back(vertex);

    vertex.position = { 0.5f, -0.5f, -1.0f };
    vertex.texCoords = { 0,1 };
    vertices.push_back(vertex);

    vertex.position = { 0.5f,  0.5f, -1.0f };
    vertex.texCoords = { 0,0 };
    vertices.push_back(vertex);
    
    vertex.position = { -0.5f, -0.5f, -1.0f };
    vertex.texCoords = { 1,0 };
    vertices.push_back(vertex);

    vertex.position = { -0.5f,  0.5f, -1.0f };
    vertex.texCoords = { 1,1 };
    vertices.push_back(vertex);


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
    indices.insert(indices.begin(), in, in + 12 * 3);
    memcpy(indices.data(), in, 12 * 3);

    //std::copy(v, v + verticesCount, vertices);

    //indices = new uint32_t[12 * 3];
    //indicesCount = 12 * 3;
    //uint32_t in[] =
    //{
    //   0,1,2,
    //   2,3,0,

    //   3,2,4,
    //   4,5,3,

    //   5,4,6,
    //   6,7,5,

    //   7,6,1,
    //   1,0,7,

    //   3,5,7,
    //   7,0,3,

    //   1,6,4,
    //   4,2,1

    //};
    //std::copy(in, in + indicesCount, indices);


    //texCoordsCount = 8;
    //texCoords = new float2[texCoordsCount];
    ///*float2 tc[] =
    //{
    //    {1,1},
    //    {1,0},
    //    {0,0},
    //    {0,1},

    //    {0,0},
    //    {1,0},
    //    {1,1},
    //    {0,1},
    //};*/

    //float2 tc[] =
    //{
    //    {0,1},
    //    {0,0},
    //    {1,0},
    //    {1,1},

    //    {0,1},
    //    {0,0},
    //    {1,0},
    //    {1,1}
    //};

    //std::copy(tc, tc + texCoordsCount, texCoords);

    SetUpBuffers();
}

KbCube::~KbCube()
{
    //RELEASE_ARRAY(vertices);
    //RELEASE_ARRAY(indices);
    RELEASE_ARRAY(normals);
}
