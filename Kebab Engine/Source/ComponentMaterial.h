#pragma once

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject& compOwner);
	~ComponentMaterial();

	void Enable();
	void Disable();

	void DrawOnInspector();

private:

};