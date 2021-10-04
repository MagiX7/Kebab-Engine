#include "Cube.h"

Cube::Cube(math::float3 pos, math::float3 size)
{
    vertices.resize(36 * 3);
    vertices.reserve(36 * 3);
    //indices.resize(36);
    // Without indices
    float v[] = {
        // Front face
        pos.x, pos.y, pos.z,          // Top Left
        pos.x, pos.y + size.y,pos.z,  // Bottom left
        pos.x + size.x, pos.y,pos.z,  // Top Right
        pos.x, pos.y + size.y,pos.z,  // Bottom left
        pos.x + size.x, pos.y,pos.z,  // Bottom right
        pos.x + size.x, pos.y,pos.z,  // Top right

        // Right face
        pos.x + size.x, pos.y + size.y,pos.z,  // Bottom right
        pos.x + size.x, pos.y,pos.z + size.z,  // Top Back Right
        pos.x + size.x, pos.y,pos.z,           // Top right
        pos.x + size.x, pos.y + size.y,pos.z,  // Bottom right
        pos.x + size.x, pos.y + size.y,pos.z + size.z,  // Bottom Back right
        pos.x + size.x, pos.y,pos.z + size.z,  // Top Back Right

        // Back face
        pos.x, pos.y, pos.z + size.z,          // Top back Left
        pos.x, pos.y + size.y,pos.z + size.z,  // Bottom back left
        pos.x + size.x, pos.y,pos.z + size.z,  // Top Back Right
        pos.x, pos.y + size.y,pos.z + size.z,  // Bottom Back left
        pos.x + size.x, pos.z + size.z,        // Bottom Back right
        pos.x + size.x, pos.y,pos.z + size.z,  // Top Back right

        // Left face
        pos.x, pos.y, pos.z,                   // Top left
        pos.x, pos.y + size.y,pos.z,           // Bottom left
        pos.x, pos.y, pos.z + size.z,          // Top back Left
        pos.x, pos.y + size.y,pos.z + size.z,  // Bottom back left
        pos.x + size.x, pos.y + size.y,pos.z + size.z,  // Bottom Back right
        pos.x, pos.y, pos.z + size.z,          // Top back Left

        // Top face
        pos.x, pos.y, pos.z,                    // Top left
        pos.x + size.x, pos.y,pos.z + size.z,   // Top Back right
        pos.x, pos.y, pos.z + size.z,           // Top Back left
        pos.x, pos.y, pos.z,                    // Top left
        pos.x + size.x, pos.y,pos.z,            // Top Right
        pos.x + size.x, pos.y,pos.z + size.z,   // Top Back right

        // Bottom face
        pos.x, pos.y + size.y, pos.z,                   // Top left
        pos.x + size.x, pos.y + size.y,pos.z + size.z,  // Top Back right
        pos.x, pos.y + size.y, pos.z + size.z,          // Top Back left
        pos.x, pos.y + size.y, pos.z,                   // Top left
        pos.x + size.x, pos.y + size.y,pos.z,           // Top Right
        pos.x + size.x, pos.y + size.y,pos.z + size.z,  // Top Back right
    };

    for(int i=0;i<vertices.size();++i)
    {
        vertices[i] = v[i];
    }
}

Cube::~Cube()
{
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();

}


