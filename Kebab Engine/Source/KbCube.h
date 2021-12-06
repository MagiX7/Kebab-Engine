#pragma once

#include "KbMesh.h"

class KbCube : public KbMesh
{
public:
    KbCube(math::float3 pos, math::float3 size);

    ~KbCube();
};