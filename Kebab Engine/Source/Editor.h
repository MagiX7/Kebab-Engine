#pragma once

#include "Module.h"

#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelViewport.h"

//#define MAX_IT_HIST 100

class Editor : public Module
{
public:
	Editor(bool startEnabled = true);
	virtual ~Editor();

	bool Start() override;
	bool Update(float dt) override;
	bool Draw(float dt);
	bool CleanUp() override;

	bool OnImGuiRender(float dt, FrameBuffer* frameBuffer);

private:
	void InitImGui();
	
	void ShowAboutPanel();

	float scroll;

public:

	ConsolePanel* consolePanel;
	ConfigPanel configPanel;
	ViewportPanel* viewportPanel;

	//FrameBuffer* frameBuffer;
	//float2 viewportSize = { 0,0 };

	bool showAboutPanel;
	bool wireframe;

	/*float fpsLog[MAX_IT_HIST];
	float msLog[MAX_IT_HIST];
	int i;*/

};