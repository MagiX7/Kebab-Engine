#pragma once

#include "KbMesh.h"

//#include "ComponentMesh.h"

class KbCube : public KbMesh
{
public:
    KbCube(math::float3 pos, math::float3 size);

    ~KbCube();

    //void Draw() override;
};