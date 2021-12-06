#pragma once

#include "Component.h"

#include <vector>
#include <memory>

class Texture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* compOwner);
	~ComponentMaterial();

	void Bind();
	void Unbind();

	void Enable();
	void Disable();

	void DrawOnInspector();

	void ShowTexturesMenu();

	void AddTexture(std::shared_ptr<Texture>tex, int modelUuid);

	inline Texture* GetCurrentTexture() { return currentTexture; }

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;

private:
	void SetCheckersTexture();

private:

	std::shared_ptr<Texture> texture;
	bool unselect = false;

	Texture* currentTexture;
	std::shared_ptr<Texture> checkersTexture;

	bool menuSelectTex;
	std::vector<Texture*> textures;
};