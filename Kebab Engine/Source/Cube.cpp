#include "Cube.h"

#include "Globals.h"

#include "mmgr/mmgr.h"

KbCube::KbCube(math::float3 pos, math::float3 size, GameObject* owner) : KbGeometry(owner)
{
    isKbGeometry = true;


    Vertex vertex;
    
    //// 0
    //vertex.position = { -0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 0,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 1
    //vertex.position = { -0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 0,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 2
    //vertex.position = { 0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);


    //// 2
    //vertex.position = { 0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 3
    //vertex.position = { 0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 0
    //vertex.position = { -0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 0,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);


    //// 2
    //vertex.position = { 0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 3
    //vertex.position = { 0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 4
    //vertex.position = { 0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 0,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);


    //// 4
    //vertex.position = { 0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 0,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 5
    //vertex.position = { 0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 0,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 3
    //vertex.position = { 0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);

    //// 5
    //vertex.position = { 0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 0,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 4
    //vertex.position = { 0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 0,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 6
    //vertex.position = { -0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);

    //// 6
    //vertex.position = { -0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 7
    //vertex.position = { -0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 5
    //vertex.position = { 0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 0,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);

    //// 7
    //vertex.position = { -0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 6
    //vertex.position = { -0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 1
    //vertex.position = { -0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 0,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);


    //// 1
    //vertex.position = { -0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 0,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 0
    //vertex.position = { -0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 0,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 7
    //vertex.position = { -0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);

    //// 7
    //vertex.position = { -0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 0
    //vertex.position = { -0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 0,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 3
    //vertex.position = { 0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);

    //// 3
    //vertex.position = { 0.5f,  0.5f,  0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 5
    //vertex.position = { 0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 0,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 7
    //vertex.position = { -0.5f,  0.5f, -0.5f };
    //vertex.texCoords = { 1,0 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);

    //// 6
    //vertex.position = { -0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 1
    //vertex.position = { -0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 0,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 2
    //vertex.position = { 0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //
    //// 2
    //vertex.position = { 0.5f, -0.5f,  0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 4
    //vertex.position = { 0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 0,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);
    //// 6
    //vertex.position = { -0.5f, -0.5f, -0.5f };
    //vertex.texCoords = { 1,1 };
    //vertex.normal = { vertex.position };
    //vertices.emplace_back(vertex);


    //indices.resize(8 * 3);
    //for (int i = 0; i < 8 * 3; ++i) indices[i] = i;


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

    SetUpBuffers();
}

KbCube::~KbCube()
{
    //RELEASE_ARRAY(vertices);
    //RELEASE_ARRAY(indices);
    delete[](normals);
}

//void KbCube::Draw()
//{
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//
//    vertexBuffer->Bind();
//    indexBuffer->Bind();
//    glVertexPointer(3, GL_FLOAT, 0, 0);
//
//    texture->Bind();
//    glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
//    glTexCoordPointer(2, GL_FLOAT, 0, 0);
//
//    glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
//
//    if (drawVertexNormals)
//        DrawVertexNormals();
//    if (drawTriangleNormals)
//        DrawTriangleNormals();
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    texture->Unbind();
//
//    vertexBuffer->Unbind();
//    indexBuffer->Unbind();
//
//    //vertexArray->Unbind();
//
//    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//    glDisableClientState(GL_VERTEX_ARRAY);
//}
