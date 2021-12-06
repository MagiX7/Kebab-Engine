#pragma once

#include "KbMesh.h"

class KbSphere : public KbMesh
{
public:
	// Stack is for the verticals division and sectors is for the horizontal divisions
	KbSphere(float3 pos, float radius, float rings, float sectors);
};