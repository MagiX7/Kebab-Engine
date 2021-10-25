#pragma once

#include "Component.h"

#include <string>
#include <vector>

#include "Geometry/AABB.h"

class GameObject
{
public:
	GameObject(std::string name);
	~GameObject();

	void Update(float dt);

	Component* CreateComponent(ComponentType type);
	Component* GetComponent(ComponentType type);

	inline void SetParent(GameObject* newParent) { parent = newParent; }
	inline GameObject* GetParent() const { return parent; }
	inline const bool& HasParent() const { return parent; }

	void AddComponent(Component* comp);
	void AddChild(GameObject* child);

	inline const std::vector<Component*>& GetComponents() const { return components; }
	inline std::vector<GameObject*>& GetChilds() { return childs; }
	inline const std::string GetName() const { return name; }

	void AddAABB();
	AABB* GetLocalAABB();
	void SetCompleteAABB(GameObject* parent);
	AABB* GetCompleteAABB();

	bool activeAABB;

private:

	std::string name;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> childs;

	AABB aabb;
	AABB* localAABB;

	bool active;
};