#pragma once

#include "Component.h"

#include "Parser.h"

#include <string>
#include <vector>

#include "Geometry/AABB.h"

class GameObject
{
public:
	GameObject(std::string name, int uuid = 0);
	virtual ~GameObject();

	void Update(float dt);

	Component* CreateComponent(ComponentType type, std::string meshPath = "");
	Component* GetComponent(ComponentType type);

	const int& GetUuid() const { return uuid; }

	inline void SetParent(GameObject* newParent) { parent = newParent; }
	void UnParent();
	inline GameObject* GetParent() const { return parent; }
	inline const bool& HasParent() const { return parent; }

	inline bool HasChilds() { return childs.size() > 0; }
	void EraseChild(GameObject* go);

	void AddComponent(Component* comp);
	void AddChild(GameObject* child);

	inline void SetUUID(const int& newUUID) { uuid = newUUID; }

	inline const std::vector<Component*>& GetComponents() const { return components; }
	inline std::vector<GameObject*>& GetChilds() { return childs; }
	inline const std::string GetName() const { return name; }

	inline void SetName(char* newName) { name = newName; }

	void AddAABB();
	AABB* GetLocalAABB();
	void SetGlobalAABB(GameObject* parent);
	void SetGlobalAABB(const AABB& aabb);
	AABB* GetGlobalAABB();
	void UpdateAABB(float4x4& newTrans);

	void HasMoved();
	void PropagateTransform();

	bool insideFrustum;
	bool isStatic;
	bool inQT;

	void Save(JSON_Array* array);
	JSON_Value* Load(JSON_Object* obj);
	
private:
	void LoadComponents(JSON_Array* compsArray, GameObject* parent);

	int uuid;

	std::string name;
	std::vector<Component*> components;

	GameObject* parent;
	std::vector<GameObject*> childs;

	AABB globalAABB;
	AABB localAABB;

	bool active;
};