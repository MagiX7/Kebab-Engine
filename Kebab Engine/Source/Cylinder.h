#pragma once

#include "Geometry.h"

class KbCylinder : public KbGeometry
{
public:
	KbCylinder(math::float3 pos, float baseRadius, float height, float segments, GameObject* owner);
	virtual ~KbCylinder();

};