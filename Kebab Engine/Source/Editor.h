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
class GameDebugInfoPanel;
class FrameBuffer;
class Texture;

class MainScene;

enum class SceneState
{
	EDIT = 0,
	PLAY,
	PAUSE,
	STEP_ONE_FRAME
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

	void SerializeScene(const char* path);
	void UnserializeScene(const char* path);

	void OnScenePlay();
	void OnSceneStop();
	void OnScenePause();
	void OnSceneResume();
	void OnSceneStepFrame();

	inline const SceneState& GetSceneState() { return sceneState; }
	inline void SetSceneState(const SceneState& newState) { sceneState = newState; }

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
	GameDebugInfoPanel* gameDebugInfoPanel;

	bool showAboutPanel;
	bool wireframe;

private:
	std::string currentSaveDirectory;
	std::string currentLoadDirectory;

	//float lastRuntimeDt;
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