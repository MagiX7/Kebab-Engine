#pragma once

#include "Globals.h"

class GameObject;

enum class ComponentType
{
	TRANSFORM,
	MESH,
	MATERIAL,
};

class Component
{
public:
	virtual void Enable() {};
	virtual void Disable() {};

	virtual void Update() {};

	virtual inline bool IsActive() { return active; }
	virtual inline void SetActive(bool value) { active = value; }

	inline void SetOwner(GameObject* newOwner) { owner = newOwner; }

	inline const ComponentType& GetComponentType() const { return type; }

protected:

	ComponentType type;
	GameObject* owner;

	bool active;
};