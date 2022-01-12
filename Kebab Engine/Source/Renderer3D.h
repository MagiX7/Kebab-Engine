#pragma once

#include "Module.h"

#include "Geometry.h"
#include "MeshLoader.h"

#include "Globals.h"
#include "Light.h"
#include "Lights.h"

#define MAX_LIGHTS 8

class ComponentMesh;
class ComponentCamera;

class Material;
class Shader;

class Renderer3D : public Module
{
public:
	Renderer3D(bool startEnabled = true);
	~Renderer3D();

	bool Init(JSON_Object* root);
	bool Start() override;
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
	void SetBlending();

	void Save(JSON_Object* root) override;
	void Load(JSON_Object* root) override;

	void Submit(GameObject* go);
	void EraseGameObject(GameObject* go);
	void EraseAllGameObjects();

	void DrawGrid();
	void DrawAABB(AABB& aabb);


	Material* GetDefaultMaterial();
	Shader* GetDefaultShader();

	Shader* AddShader(const std::string& path);
	void AddMaterial(Material* material);
	inline const std::vector<Shader*>& GetShaders() { return shaders; }

	void AddPointLight(PointLight* pl = &PointLight());
	void DeletePointLight(PointLight* pl);
	inline const std::vector<PointLight*>& GetPointLights() { return pLights; }
	
	void AddSpotLight(SpotLight* sl = &SpotLight());
	inline const std::vector<SpotLight*>& GetSpotLights() { return spotLights; }
	

private:
	void DoRender(bool gameScene);
	void PushCamera(ComponentCamera* cam);

public:

	OldLight lights[MAX_LIGHTS];
	SDL_GLContext context;

	bool depth;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool wireframe;
	bool blend;

	bool drawAABB;
	bool drawGrid;

	GameObject* goDirLight;
	DirectionalLight* dirLight;

	bool gammaCorrection;
	float gammaCorrectionAmount;

private:
	std::vector<PointLight*> pLights;
	std::vector<SpotLight*> spotLights;


	std::vector<ComponentMesh*> meshes;
	std::vector<GameObject*> gameObjects;
	
	FrameBuffer* editorFbo;
	FrameBuffer* sceneFbo;

	Material* defaultMaterial;
	Shader* defaultShader;
	std::vector<Shader*> shaders;
	std::vector<Material*> materials;

	float camsActive;
};