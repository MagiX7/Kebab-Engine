#pragma once

#include "Module.h"

#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelViewport.h"
#include "PanelHierarchy.h"
#include "PanelInspector.h"

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

	void SetImGuiStyle();

public:

	ConsolePanel* consolePanel;
	ConfigPanel* configPanel;
	ViewportPanel* viewportPanel;
	HierarchyPanel* hierarchyPanel;
	InspectorPanel* inspectorPanel;

	bool showAboutPanel;
	bool wireframe;

	/*float fpsLog[MAX_IT_HIST];
	float msLog[MAX_IT_HIST];
	int i;*/

private:
	float scroll;

	bool showWindows;

};