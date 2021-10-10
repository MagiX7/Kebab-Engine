#pragma once

#include "Module.h"

#include "PanelConfiguration.h"
#include "PanelConsole.h"

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


private:
	void InitImGui();
	bool OnImGuiRender(float dt);
	
	void ShowAboutPanel();

public:

	ConsolePanel* consolePanel;
	ConfigPanel configPanel;

	FrameBuffer* frameBuffer;
	float2 viewportSize = { 0,0 };

	bool showAboutPanel;
	bool wireframe;

	/*float fpsLog[MAX_IT_HIST];
	float msLog[MAX_IT_HIST];
	int i;*/

};