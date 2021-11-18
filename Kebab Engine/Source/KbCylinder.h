#pragma once

#include "KbMesh.h"

class KbCylinder : public KbMesh
{
public:
	KbCylinder(math::float3 pos, float baseRadius, float height, float sectors);
	virtual ~KbCylinder();

	std::vector<Vertex> GetUnitCircleVertices();
	void BuildVerticesSmooth();

private:
	int sectors;
	float height;
	float radius;

	int baseCenterIndex;
	int topCenterIndex;
};