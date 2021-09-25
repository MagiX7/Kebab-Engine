#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Camera3D : public Module
{
public:
	Camera3D(Application* app, bool startEnabled = true);
	~Camera3D();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 x, y, z, position, reference;

private:

	mat4x4 viewMatrix, viewMatrixInverse;
};