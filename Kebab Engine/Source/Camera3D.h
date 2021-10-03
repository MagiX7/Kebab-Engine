#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Camera3D : public Module
{
public:
	Camera3D(bool startEnabled = true);
	~Camera3D();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Look(const vec3& pos, const vec3 &ref, bool rotateAroundReference = false);
	void LookAt(const vec3& spot);
	void Move(const vec3& movement);
	float* GetViewMatrix();

	void Save(JSON_Object* root) override;

private:

	void CalculateViewMatrix();

public:
	
	vec3 x, y, z, position, reference;

private:

	mat4x4 viewMatrix, viewMatrixInverse;
};