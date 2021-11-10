#pragma once

#include "Panel.h"

class GameObject;

class HierarchyPanel : public Panel
{
public:
	HierarchyPanel();
	~HierarchyPanel();

	void OnRender(float dt) override;
	void SetCurrent(GameObject* go);

private:
	void DisplayHierarchy(GameObject* go);
	void DisplayGameObjectMenu(GameObject* go);

public:
	GameObject* currentGO;

private:
	GameObject* goDragging;
	GameObject* goClicked;
	bool optionsPopup;
	bool parentingPopup;

	bool selectedFromViewport;
};