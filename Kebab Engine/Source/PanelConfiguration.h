#pragma once

#include "Panel.h"
#include "Window.h"
#include "Renderer3D.h"

#define MAX_IT_HIST 100

class ConfigPanel : public Panel
{
public:
	ConfigPanel();
	~ConfigPanel();

	bool Update(float dt) override;
	void Draw() override;

	Window* windowConfig;
	Renderer3D* renderConfig;

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
	bool depth;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	// TODO: Other two
};