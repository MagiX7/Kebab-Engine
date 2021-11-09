#pragma once

#include "Component.h"

#include <vector>

class Texture;

class ComponentMaterial : public Component
{
public:
	// Loads checkers texture by default
	ComponentMaterial(GameObject& compOwner);
	~ComponentMaterial();

	void Bind();
	void Unbind();

	void Enable();
	void Disable();

	void DrawOnInspector();

	void AddTexture(Texture* tex);

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;

private:
	void SetCheckersTexture();

private:
	std::vector<Texture*> textures;

	Texture* currentTexture;
	Texture* texture;
	Texture* checkersTexture;

};