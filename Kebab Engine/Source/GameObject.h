#pragma once

#include "Component.h"

#include <string>
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update(float dt);
	Component* CreateComponent(ComponentType type);

private:

	std::string* name;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> children;

	bool active;
};