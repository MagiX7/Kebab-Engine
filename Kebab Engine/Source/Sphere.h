#pragma once

#include "Geometry.h"

class KbSphere : public KbGeometry
{
public:
	// Stack is for the verticals division and sectors is for the horizontal divisions
	KbSphere(float3 pos, float radius, float rings, float sectors, GameObject* owner);

	//void Draw(bool drawVertexNormals, bool drawTriangleNormals) override;
};