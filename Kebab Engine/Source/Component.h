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

protected:

	ComponentType type;
	GameObject* owner;

	bool active;
};