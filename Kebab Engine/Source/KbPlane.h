#pragma once

#include "KbMesh.h"

class KbPlane : public KbMesh
{
public:
	// Position is the top left corner.
	KbPlane(float3 position, float2 size);

	~KbPlane();

	//void Draw(bool drawVertexNormals, bool drawTriangleNormals) override;
};