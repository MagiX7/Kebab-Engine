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
	Component* GetComponent(ComponentType type);

	inline void SetParent(GameObject* newParent) { parent = newParent; }

	void AddComponent(Component* comp);
	void AddChild(GameObject* child);

	inline const std::vector<Component*>& GetComponents() const { return components; }
	inline const std::vector<GameObject*>& GetChilds() const { return childs; }


private:

	std::string* name;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> childs;

	bool active;
};