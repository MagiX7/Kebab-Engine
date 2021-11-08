#pragma once

#include "Component.h"

#include <string>
#include <vector>

#include "Geometry/AABB.h"

class GameObject
{
public:
	GameObject(std::string name);
	virtual ~GameObject();

	void Update(float dt);

	Component* CreateComponent(ComponentType type);
	Component* GetComponent(ComponentType type);

	inline void SetParent(GameObject* newParent) { parent = newParent; }
	void UnParent();
	inline GameObject* GetParent() const { return parent; }
	inline const bool& HasParent() const { return parent; }

	inline bool HasChilds() { return childs.size() > 0; }
	void EraseChild(GameObject* go);

	void AddComponent(Component* comp);
	void AddChild(GameObject* child);


	inline const std::vector<Component*>& GetComponents() const { return components; }
	inline std::vector<GameObject*>& GetChilds() { return childs; }
	inline const std::string GetName() const { return name; }

	void AddAABB();
	AABB* GetLocalAABB();
	void SetGlobalAABB(GameObject* parent);
	AABB* GetGlobalAABB();
	void UpdateAABB(float4x4& newTrans);


private:

	std::string name;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> childs;

	AABB globalAABB;
	AABB localAABB;

	bool active;
};