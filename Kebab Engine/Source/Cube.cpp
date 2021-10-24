#include "Cube.h"

#include "Globals.h"

KbCube::KbCube(math::float3 pos, math::float3 size, GameObject* owner) : KbGeometry(owner)
{
    Vertex vertex;

    vertex.position = { -0.5f,  0.5f,  0.0f };
    vertex.texCoords = { 0,1 };
    vertex.normal = { vertex.position };
    vertices.push_back(vertex);

    vertex.position = { -0.5f, -0.5f,  0.0f };
    vertex.texCoords = { 0,0 };
    vertex.normal = { vertex.position };
    vertices.push_back(vertex);

    vertex.position = { 0.5f, -0.5f,  0.0f };
    vertex.texCoords = { 1,0 };
    vertex.normal = { vertex.position };
    vertices.push_back(vertex);

    vertex.position = { 0.5f,  0.5f,  0.0f };
    vertex.texCoords = { 1,1 };
    vertex.normal = { vertex.position };
    vertices.push_back(vertex);

    vertex.position = { 0.5f, -0.5f, -1.0f };
    vertex.texCoords = { 0,1 };
    vertex.normal = { vertex.position };
    vertices.push_back(vertex);

    vertex.position = { 0.5f,  0.5f, -1.0f };
    vertex.texCoords = { 0,0 };
    vertex.normal = { vertex.position };
    vertices.push_back(vertex);
    
    vertex.position = { -0.5f, -0.5f, -1.0f };
    vertex.texCoords = { 1,0 };
    vertex.normal = { vertex.position };
    vertices.push_back(vertex);

    vertex.position = { -0.5f,  0.5f, -1.0f };
    vertex.texCoords = { 1,1 };
    vertex.normal = { vertex.position };
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

    SetUpBuffers();
}

KbCube::~KbCube()
{
    //RELEASE_ARRAY(vertices);
    //RELEASE_ARRAY(indices);
    RELEASE_ARRAY(normals);
}

void KbCube::Draw(bool drawVertexNormals, bool drawTriangleNormals)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);


    vertexBuffer->Bind();
    indexBuffer->Bind();
    glVertexPointer(3, GL_FLOAT, 0, 0);

    texture->Bind();
    glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

    if (drawVertexNormals)
        DrawVertexNormals();
    if (drawTriangleNormals)
        DrawTriangleNormals();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    texture->Unbind();

    vertexBuffer->Unbind();
    indexBuffer->Unbind();

    //vertexArray->Unbind();

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
