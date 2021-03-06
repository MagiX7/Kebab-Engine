#include "KbCylinder.h"

#include "mmgr/mmgr.h"

KbCylinder::KbCylinder(math::float3 pos, float baseRadius, float height, float sectors)
{
    this->height = height;
    this->radius = baseRadius;
    this->sectors = sectors;

    BuildVerticesSmooth();

    int k1 = 0;              // 1st vertex index at base
    int k2 = sectors + 1;    // 1st vertex index at top

    for (int i = 0; i < sectors; ++i, ++k1, ++k2)
    {
        // 2 triangles per sector
        // k1 => k1+1 => k2
        indices.push_back(k1);
        indices.push_back(k1 + 1);
        indices.push_back(k2);

        // k2 => k1+1 => k2+1
        indices.push_back(k2);
        indices.push_back(k1 + 1);
        indices.push_back(k2 + 1);
    }

    for (int i = 0, k = baseCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            indices.push_back(baseCenterIndex);
            indices.push_back(k + 1);
            indices.push_back(k);
        }
        else // last triangle
        {
            indices.push_back(baseCenterIndex);
            indices.push_back(baseCenterIndex + 1);
            indices.push_back(k);
        }
    }

    // indices for the top surface
    for (int i = 0, k = topCenterIndex + 1; i < sectors; ++i, ++k)
    {
        if (i < sectors - 1)
        {
            indices.push_back(topCenterIndex);
            indices.push_back(k);
            indices.push_back(k + 1);
        }
        else // last triangle
        {
            indices.push_back(topCenterIndex);
            indices.push_back(k);
            indices.push_back(topCenterIndex + 1);
        }
    }

    SetUpMesh();
}

KbCylinder::~KbCylinder()
{

}

std::vector<Vertex> KbCylinder::GetUnitCircleVertices()
{
    float sectorStep = 2 * pi / sectors;
    float sectorAngle;  // radian

    std::vector<Vertex> unitCircleVertices;
    for (int i = 0; i <= sectors; ++i)
    {
        sectorAngle = i * sectorStep;
        
        Vertex vertex;

        vertex.position.x = cos(sectorAngle);
        vertex.position.y = sin(sectorAngle);
        vertex.position.z = 0;

        unitCircleVertices.push_back(vertex);
    }
    return unitCircleVertices;
}

void KbCylinder::BuildVerticesSmooth()
{
    // get unit circle vectors on XY-plane
    std::vector<Vertex> unitVertices = GetUnitCircleVertices();

    // put side vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float t = 1.0f - i;                              // vertical tex coord; 1 to 0

        for (int j = 0, k = 0; j <= sectors; ++j, ++k)
        {
            Vertex vertex;

            float ux = unitVertices[k].position.x;
            float uy = unitVertices[k].position.y;
            float uz = unitVertices[k].position.z;

            // position vector
            vertex.position = { ux * radius,uy * radius,h };

            // normal vector
            vertex.normal = { ux,uy,uz };

            // texture coordinate
            vertex.texCoords = { (float)j / sectors , t };
        
            vertices.push_back(vertex);
        }
    }

    // the starting index for the base/top surface
    //NOTE: it is used for generating indices later
    baseCenterIndex = (int)vertices.size() / 3;
    topCenterIndex = baseCenterIndex + sectors + 1; // include center vertex

    // put base and top vertices to arrays
    for (int i = 0; i < 2; ++i)
    {
        Vertex vertex;
        
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float nz = -1 + i * 2;                           // z value of normal; -1 to 1

        // center point
        vertex.position = { 0,0,h };
        vertex.normal = { 0,0,nz };
        vertex.texCoords = { 0.5f, 0.5f };

        vertices.push_back(vertex);

        for (int j = 0, k = 0; j < sectors; ++j, ++k)
        {
            Vertex vertex;

            float ux = unitVertices[k].position.x;
            float uy = unitVertices[k].position.y;

            vertex.position = { ux * radius, uy * radius,h };
            vertex.normal = { 0,0,nz };
            vertex.texCoords = { -ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f };
            
            vertices.push_back(vertex);
        }
    }
}
