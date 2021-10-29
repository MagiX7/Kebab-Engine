#pragma once

#include "Panel.h"

class HierarchyPanel : public Panel
{
public:
	HierarchyPanel();
	~HierarchyPanel();

	void OnRender(float dt) override;

private:
	void DisplayHierarchy(GameObject* go);
	void DisplayGameObjectMenu(GameObject* go);

	bool IsMouseInside(float4 bounds);

public:
	GameObject* currentGO;

private:
	GameObject* goDragging;
	GameObject* goClicked;
	bool optionsPopup;
};