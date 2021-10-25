#pragma once

#include "Geometry.h"

class KbPyramid : public KbGeometry
{
public:
	// Position is the middle of the base
	KbPyramid(float3 pos, float height, float baseWidth, GameObject* parent);

	virtual ~KbPyramid();

	void Draw(bool drawVertexNormals, bool drawTriangleNormals) override;
};