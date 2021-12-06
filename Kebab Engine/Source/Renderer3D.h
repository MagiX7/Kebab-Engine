#pragma once

#include "Module.h"

#include "Geometry.h"
#include "MeshLoader.h"

#include "Globals.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ComponentMesh;
class ComponentCamera;

class Renderer3D : public Module
{
public:
	Renderer3D(bool startEnabled = true);
	~Renderer3D();

	bool Init(JSON_Object* root);
	bool PreUpdate(float dt);
	bool Draw(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	
	void SetDepth();
	void SetCullFace();
	void SetLighting();
	void SetColorMaterial();
	void SetTexture2D();
	void SetWireframe();

	void Save(JSON_Object* root) override;
	void Load(JSON_Object* root) override;

	void Submit(GameObject* go);
	void EraseGameObject(GameObject* go);
	void EraseAllGameObjects();

	void DrawGrid();
	void DrawAABB(AABB& aabb);

private:
	void DoRender(bool gameScene);
	void PushCamera(ComponentCamera* cam);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;

	bool depth;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool wireframe;

	bool drawAABB;
	bool drawGrid;

private:
	std::vector<ComponentMesh*> meshes;
	std::vector<GameObject*> gameObjects;
	
	FrameBuffer* editorFbo;
	FrameBuffer* sceneFbo;

	float camsActive;
};