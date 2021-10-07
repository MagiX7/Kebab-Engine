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
	bool Draw(float dt) override;
	bool CleanUp() override;


private:
	void InitImGui();
	bool UpdateImGui(float dt);
	void DrawImGui();
	
	void ShowAboutPanel();

public:

	ConsolePanel* consolePanel;
	ConfigPanel configPanel;

	bool showAboutPanel;
	bool wireframe;

	/*float fpsLog[MAX_IT_HIST];
	float msLog[MAX_IT_HIST];
	int i;*/

};