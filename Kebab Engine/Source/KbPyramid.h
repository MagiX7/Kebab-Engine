#pragma once

#include "KbMesh.h"

class KbPyramid : public KbMesh
{
public:
	// Position is the middle of the base
	KbPyramid(float3 pos, float height, float baseWidth);

	virtual ~KbPyramid();

	//void Draw() override;
};