#pragma once

#include "Geometry.h"

class KbCylinder : public KbGeometry
{
public:
	KbCylinder(math::float3 pos, float baseRadius, float height, float sectors, GameObject* owner);
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