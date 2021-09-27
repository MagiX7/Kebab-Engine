#pragma once

#include "Panel.h"

#define MAX_IT_HIST 100

class ConfigPanel : public Panel
{
public:
	ConfigPanel();
	~ConfigPanel();

	bool Update(float dt) override;
	void Draw() override;

private:
	float fpsLog[MAX_IT_HIST];
	float msLog[MAX_IT_HIST];
	float memCost[MAX_IT_HIST];
	int i;
};