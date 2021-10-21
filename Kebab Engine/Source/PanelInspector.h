#pragma once

#include "Panel.h"

#include "GameObject.h"

class InspectorPanel : public Panel
{
public:
	InspectorPanel();
	virtual ~InspectorPanel();

	void OnRender(float dt) override;

private:
	void DisplayTransform(GameObject* go);
	void DisplayMeshInfo(GameObject* go);
	void DisplayTextureInfo(GameObject* go);

};