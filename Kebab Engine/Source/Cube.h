#pragma once

#include "Geometry.h"

class Cube : public Geometry
{
public:
	Cube(math::float3 pos, math::float3 size);
	virtual ~Cube();

};