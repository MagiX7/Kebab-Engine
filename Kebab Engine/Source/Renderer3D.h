#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class Renderer3D : public Module
{
public:
	Renderer3D(Application* app, bool start_enabled = true);
	~Renderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};