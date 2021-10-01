#pragma once

#include "Module.h"

#include "PanelConfiguration.h"
#include "PanelConsole.h"

//#define MAX_IT_HIST 100

class Editor : public Module
{
public:
	Editor(Application* app, bool startEnabled = true);
	virtual ~Editor();

	bool Start() override;
	bool Update(float dt) override;
	bool Draw(float dt) override;
	bool CleanUp() override;


private:
	void ShowAboutPanel();

public:

	ConsolePanel consolePanel;
	ConfigPanel configPanel;

	bool showAboutPanel;

	/*float fpsLog[MAX_IT_HIST];
	float msLog[MAX_IT_HIST];
	int i;*/

};