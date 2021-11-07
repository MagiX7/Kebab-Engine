#pragma once

#include "Globals.h"
#include "Parser.h"

class GameObject;

enum class ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
};

class Component
{
public:
	virtual ~Component() {};

	virtual void Enable() {};
	virtual void Disable() {};

	virtual void Update() {};

	virtual void DrawOnInspector() {};

	virtual inline bool IsActive() { return active; }
	virtual inline void SetActive(bool value) { active = value; }

	inline void SetParent(GameObject* newParent) { parent = newParent; }
	inline const GameObject* GetParent() const { return parent; }

	inline const ComponentType& GetComponentType() const { return type; }

	virtual JSON_Value* Save(JSON_Object* goObj) { return nullptr; };

protected:
	int uuid;

	ComponentType type;
	GameObject* parent;

	bool active;
};