#pragma once

#include "Geometry.h"

class KbCube : public KbGeometry
{
public:
    KbCube(math::float3 pos, math::float3 size);

    ~KbCube();
};