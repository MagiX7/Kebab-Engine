#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include "Buffer.h"

#define MAX_LIGHTS 8

class Renderer3D : public Module
{
public:
	Renderer3D(Application* app, bool startEnabled = true);
	~Renderer3D();

	bool Init();
	bool PreUpdate(float dt);
	bool Draw(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	
	void SetDepth(bool value);
	void SetCullFace(bool value);
	void SetLighting(bool value);
	void SetColorMaterial(bool value);
	void SetTexture2D(bool value);

	void Save(JSON_Object* root) override;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;

private:
	VertexBuffer* vbo;
	IndexBuffer* ibo;

	bool depth;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
};