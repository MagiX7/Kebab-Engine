#pragma once

#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject& compOwner);
	~ComponentTransform();

	void Enable();
	void Disable();

	void Update();

private:

};