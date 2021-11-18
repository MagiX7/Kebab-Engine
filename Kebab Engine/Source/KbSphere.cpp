#include "KbSphere.h"

// Slices for the X, stacks for the Y
KbSphere::KbSphere(float3 pos, float radius, float stacks, float slices)
{
    Vertex vertex;

    // Calc the vertices
    for (int i = 0; i <= stacks; ++i) {

        float v = i / (float)stacks;
        float phi = v * pi;

        // Loop through slices
        for (int j = 0; j <= slices; ++j) {

            float u = j / (float)slices;
            float theta = u * (pi * 2);

            // Calc the vertex positions
            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);

            vertex.position = (float3(x, y, z) * radius);
            vertex.texCoords = { x * radius,y * radius };
            vertices.push_back(vertex);
        }
    }

    // Calc the index positions
    for (int i = 0; i < slices * stacks + slices; ++i) {

        indices.push_back(i);
        indices.push_back(i + slices + 1);
        indices.push_back(i + slices);

        indices.push_back(i + slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    SetUpMesh();
}

//void KbSphere::Draw(bool drawVertexNormals, bool drawTriangleNormals)
//{
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//    //vertexArray->Bind();
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