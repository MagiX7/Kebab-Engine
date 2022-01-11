#pragma once

#include "Component.h"

#include "Lights.h"

class ComponentLight : public Component
{
public:
	ComponentLight();
	virtual ~ComponentLight();

	void DrawOnInspector() override;

	void SetLight(Light* light);

private:
	Light* light;

};