#pragma once

#include "Geometry.h"

class KbPlane : public KbGeometry
{
public:
	// Position is the top left corner.
	KbPlane(float3 position, float2 size, GameObject* owner);

	~KbPlane();

	//void Draw(bool drawVertexNormals, bool drawTriangleNormals) override;
};