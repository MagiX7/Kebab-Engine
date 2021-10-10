#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Camera;

class Camera3D : public Module
{
public:
	Camera3D(bool startEnabled = true);
	~Camera3D();

	bool Init(JSON_Object* root) override;
	bool Start() override;
	bool Update(float dt);
	bool CleanUp();

	void LookAt(const float3& point);
	void MoveTo(const float3& movement);
	void SetPosLook(const float3& pos, float3& pointLook);
	float* GetViewMatrix();

	void Save(JSON_Object* root) override;

	float3 reference, position;

private:

	Camera* cam;
};