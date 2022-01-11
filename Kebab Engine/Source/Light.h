
#pragma once
#include "Color.h"
//#include "glmath.h"
#include "Math/float3.h"
struct OldLight
{
	OldLight();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	float3 position;

	int ref;
	bool on;
};