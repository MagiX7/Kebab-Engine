#pragma once

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
	Component(const GameObject& owner, const ComponentType& type);
	~Component();

	virtual void Enable();
	virtual void Disable();

	virtual void Update();

private:

	ComponentType type;
	GameObject* owner;

	bool active;
};