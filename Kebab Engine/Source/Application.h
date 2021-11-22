#pragma once

#include "Module.h"

#include "Globals.h"
#include "Timer.h"

#include "parson.h"

#include <list>

//#include "mmgr.h"

class FileSystem;
class Editor;
class Renderer3D;
class Window;
class Input;
class MainScene;
class Camera3D;
class TextureManager;

class Parser;

class Application
{
public:
	Window* window;
	Input* input;
	MainScene* scene;
	Renderer3D* renderer3D;
	Camera3D* camera;
	Editor* editor;
	FileSystem* fileSystem;

private:
	std::list<Module*> modules;

	Timer msTimer;
	Timer runtimeTimer;
	//Timer startupTime;
	//Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	uint64 frameCount = 0;
	uint64 runtimeFrameCount = 0;

	float appDt;
	float runtimeDt;
	float lastRuntimeDt = 0;
	int	cappedMs = -1;
	bool stepOneFrame = false;

	bool saveReq;
	bool loadReq;
	JSON_Value* value;
	Parser* parser;

public:

	Application();
	~Application();

	bool Init();
	bool Update();
	bool CleanUp();

	void RequestSave();
	void RequestLoad();

	void SetMaxFPS(int fps);
	inline float& GetDeltaTime() { return appDt; }
	int& GetFPS();
	float& GetMaxFPS();

	inline float& GetRuntimeDt() { return runtimeDt; }
	inline void SetRuntimeDt(float newDt) { lastRuntimeDt = runtimeDt; runtimeDt = newDt; }
	inline const int& GetRuntimeFramesAmount() { return runtimeFrameCount; }
	inline void SetRuntimeFramesAmount(const int& fa) { runtimeFrameCount = fa; }
	inline const float GetLastRuntimeDt() { return lastRuntimeDt; }

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void Load();
	void Save();

};
extern Application* app;