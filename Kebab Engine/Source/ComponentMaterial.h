#pragma once

#include "Component.h"
#include "Math/float4.h"

#include <vector>
#include <memory>

class Texture;
class Material;
class ComponentCamera;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* compOwner);
	~ComponentMaterial();

	void Update(float dt) override;

	void Bind(ComponentCamera* cam);
	void Unbind();

	void Enable();
	void Disable();

	void SetMaterial(Material* mat);
	Material* GetMaterial();

	void AddTexture(std::shared_ptr<Texture>tex, int modelUuid);

	inline Texture* GetCurrentTexture() { return currentTexture; }

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;

private:

	void DrawOnInspector();
	void ShowTexturesMenu();

	void SetCheckersTexture();
	void ChangeShaderWindow();

private:
	Material* material;
	float3 ambientColor;


	std::shared_ptr<Texture> texture;
	bool unselect = false;

	Texture* currentTexture;
	std::shared_ptr<Texture> checkersTexture;

	bool menuSelectTex;

	float updateShaderTimer;
	bool closeShaderWindow;
	
};