#pragma once

#include "Geometry.h"

class KebabCube : public KebabGeometry
{
public:
	KebabCube(math::float3 pos, math::float3 size) : KebabGeometry(pos)
	{
        vertices.resize(8 * 3); // TRIANGLES
        indices.resize(12 * 3); // 4 Vertex for each face

        //float v[] = {
        //    // Front face
        //    pos.x, pos.y, pos.z,          // Top Left
        //    pos.x, pos.y + size.y,pos.z,  // Bottom left
        //    pos.x + size.x, pos.y,pos.z,  // Top Right
        //    pos.x + size.x, pos.y + size.y, pos.z,  // Bottom right
        //    pos.x + size.x, pos.y,pos.z,  // Top right
        //    pos.x, pos.y + size.y,pos.z,  // Bottom left

        //    // Right face
        //    pos.x + size.x, pos.y,pos.z,           // Top right
        //    pos.x + size.x, pos.y + size.y,pos.z,  // Bottom right
        //    pos.x + size.x, pos.y,pos.z + size.z,  // Top Back Right
        //    pos.x + size.x, pos.y + size.y,pos.z + size.z,  // Bottom Back right
        //    pos.x + size.x, pos.y,pos.z + size.z,  // Top Back Right
        //    pos.x + size.x, pos.y + size.y,pos.z,  // Bottom right

        //    // Back face
        //    pos.x, pos.y + size.y,pos.z + size.z,  // Bottom back left
        //    pos.x + size.x, pos.y + size.y,pos.z + size.z,   // Bottom Back right
        //    pos.x, pos.y, pos.z + size.z,          // Top back Left
        //    pos.x + size.x, pos.y,pos.z + size.z,  // Top Back Right
        //    pos.x, pos.y, pos.z + size.z,          // Top back Left
        //    pos.x + size.x, pos.y + size.y,pos.z + size.z,   // Bottom Back right

        //    // Left face
        //    pos.x, pos.y + size.y,pos.z,           // Bottom left
        //    pos.x, pos.y + size.y,pos.z + size.z,  // Bottom back left
        //    pos.x, pos.y, pos.z,                   // Top left
        //    pos.x, pos.y, pos.z + size.z,          // Top back Left
        //    pos.x, pos.y, pos.z + size.z,          // Top back Left
        //    pos.x, pos.y + size.y,pos.z + size.z,  // Bottom back left

        //    // Top face
        //    pos.x, pos.y, pos.z,                    // Top left
        //    pos.x + size.x, pos.y,pos.z + size.z,   // Top Back right
        //    pos.x, pos.y, pos.z + size.z,           // Top Back left
        //    pos.x, pos.y, pos.z,                    // Top left
        //    pos.x + size.x, pos.y,pos.z,            // Top Right
        //    pos.x + size.x, pos.y,pos.z + size.z,   // Top Back right

        //    // Bottom face
        //    pos.x, pos.y + size.y, pos.z,                   // Bottom left
        //    pos.x + size.x, pos.y + size.y,pos.z + size.z,  // Top Back right
        //    pos.x, pos.y + size.y, pos.z + size.z,          // Top Back left
        //    pos.x, pos.y + size.y, pos.z,                   // Top left
        //    pos.x + size.x, pos.y + size.y,pos.z,           // Top Right
        //    pos.x + size.x, pos.y + size.y,pos.z + size.z,  // Top Back right
        //};

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
	}

    virtual ~KebabCube()
    {
        vertices.clear();
        normals.clear();
        texCoords.clear();
        indices.clear();
    }
};