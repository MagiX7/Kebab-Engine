#pragma once

#include "Panel.h"

#include <string>

#define MAX_IT_HIST 100

class Renderer3D;
class Window;

class ConfigPanel : public Panel
{
public:
	ConfigPanel();
	~ConfigPanel();

	void OnRender(float dt) override;

private:
	float fpsLog[MAX_IT_HIST];
	float msLog[MAX_IT_HIST];
	float memCost[MAX_IT_HIST];
	int i;

	// Window stuff
	float brightness;
	int width;
	int height;

	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fulldesktop;

	std::string titleName;
	std::string orgName;
};