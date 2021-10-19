#pragma once

#include "Component.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject& compOwner);
	~ComponentMesh();

	void Enable();
	void Disable();

	void Update();

private:

};