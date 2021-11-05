#pragma once

#include "Module.h"

class ConfigPanel;
class ConsolePanel;
class ViewportPanel;
class InspectorPanel;
class HierarchyPanel;
class AssetsPanel;
class FrameBuffer;

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
	AssetsPanel* assetsPanel;

	bool showAboutPanel;
	bool wireframe;

private:
	float scroll;

	bool showWindows;
	bool closeApp;
};