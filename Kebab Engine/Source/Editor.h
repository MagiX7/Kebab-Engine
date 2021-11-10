#pragma once

#include "Module.h"

#include "Parser.h"

#include "ImGuizmo/ImGuizmo.h"

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

	//void Save(JSON_Object* root) override;

	void SaveScene();
	void LoadScene();

private:
	void InitImGui();
	
	void OnMainMenuRender(bool& showDemoWindow);
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

	JSON_Value* sceneValue;

	ImGuizmo::OPERATION guizmoOperation;
	ImGuizmo::MODE guizmoMode;
};