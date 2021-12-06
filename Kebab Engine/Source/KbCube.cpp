#include "KbCube.h"

#include "Globals.h"

#include "mmgr/mmgr.h"

KbCube::KbCube(math::float3 pos, math::float3 size)
{
    Vertex vertex;

    // 0
    vertex.position = { -0.5f,  0.5f,  0.5f };
    vertex.texCoords = { 0,0 };
    vertex.normal = { vertex.position };
    vertices.emplace_back(vertex);

    // 1
    vertex.position = { -0.5f, -0.5f,  0.5f };
    vertex.texCoords = { 0,1 };
    vertex.normal = { vertex.position };
    vertices.emplace_back(vertex);
    // 2
    vertex.position = { 0.5f, -0.5f,  0.5f };
    vertex.texCoords = { 1,1 };
    vertex.normal = { vertex.position };
    vertices.emplace_back(vertex);
    // 3
    vertex.position = { 0.5f,  0.5f,  0.5f };
    vertex.texCoords = { 1,0 };
    vertex.normal = { vertex.position };
    vertices.emplace_back(vertex);

    // 4
    vertex.position = { 0.5f, -0.5f, -0.5f };
    vertex.texCoords = { 0,1 };
    vertex.normal = { vertex.position };
    vertices.emplace_back(vertex);

    // 5
    vertex.position = { 0.5f,  0.5f, -0.5f };
    vertex.texCoords = { 0,0 };
    vertex.normal = { vertex.position };
    vertices.emplace_back(vertex);
    
    // 6
    vertex.position = { -0.5f, -0.5f, -0.5f };
    vertex.texCoords = { 1,1 };
    vertex.normal = { vertex.position };
    vertices.emplace_back(vertex);

    // 7
    vertex.position = { -0.5f,  0.5f, -0.5f };
    vertex.texCoords = { 1,0 };
    vertex.normal = { vertex.position };
    vertices.emplace_back(vertex);



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

    SetUpMesh();
}

KbCube::~KbCube()
{
    indices.clear();
    vertices.clear();
}