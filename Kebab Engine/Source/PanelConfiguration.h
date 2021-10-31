#pragma once

#include "Panel.h"
#include "Window.h"
#include "Renderer3D.h"

#include <string>

#define MAX_IT_HIST 100

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

	// Render stuff
	/*bool depth;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool showNormals;*/
	// TODO: Other two

	std::string titleName;
	std::string orgName;
};