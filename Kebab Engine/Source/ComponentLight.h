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

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;

private:
	Light* light;

};