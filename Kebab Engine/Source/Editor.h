#pragma once

#include "Module.h"

#include "Parser.h"

#include "ImGuizmo/ImGuizmo.h"

class ConfigPanel;
class ConsolePanel;
class ViewportPanel;
class ScenePanel;
class InspectorPanel;
class HierarchyPanel;
class AssetsPanel;
class ScenePreviewPanel;
class FrameBuffer;
class Texture;

class MainScene;

enum class SceneState
{
	EDIT = 0,
	PLAY
};

class Editor : public Module
{
public:
	Editor(bool startEnabled = true);
	virtual ~Editor();

	bool Start() override;
	bool Update(float dt) override;
	bool Draw(float dt);
	bool CleanUp() override;

	bool OnImGuiRender(float dt, FrameBuffer* frameBuffer, FrameBuffer* sceneFbo);

	//void Save(JSON_Object* root) override;

	void SerializeScene();
	void UnserializeScene();

	void OnScenePlay();
	void OnSceneStop();

	inline const SceneState& GetSceneState() { return sceneState; }

private:
	void InitImGui();
	
	void OnMainMenuRender(bool& showDemoWindow);
	void ShowAboutPanel();

	void SetImGuiStyle();

	void SimulationControl();

public:

	ConsolePanel* consolePanel;
	ConfigPanel* configPanel;
	ViewportPanel* viewportPanel;
	HierarchyPanel* hierarchyPanel;
	InspectorPanel* inspectorPanel;
	AssetsPanel* assetsPanel;
	ScenePanel* scenePanel;
	ScenePreviewPanel* previewScenePanel;

	bool showAboutPanel;
	bool wireframe;

private:
	float scroll;

	bool showWindows;
	bool closeApp;

	bool onPlay;
	
	JSON_Value* sceneValue;

	ImGuizmo::OPERATION guizmoOperation;
	ImGuizmo::MODE guizmoMode;

	Texture* playTex;
	Texture* pauseTex;
	Texture* stopTex;
	Texture* nextFrameTex;

	SceneState sceneState;
	MainScene* initialScene;
};