#pragma once

#include "Component.h"

#include <vector>
#include <memory>

class Texture;

class ComponentMaterial : public Component
{
public:
	// Loads checkers texture by default
	ComponentMaterial(GameObject* compOwner);
	~ComponentMaterial();

	void Bind();
	void Unbind();

	void Enable();
	void Disable();

	void DrawOnInspector();

	void AddTexture(std::shared_ptr<Texture>);

	inline Texture* GetCurrentTexture() { return currentTexture; }

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;

private:
	void SetCheckersTexture();

private:
	//std::vector<std::shared_ptr<Texture>> textures;

	std::shared_ptr<Texture> texture;

	Texture* currentTexture;
	//Texture* texture;
	std::shared_ptr<Texture> checkersTexture;

};