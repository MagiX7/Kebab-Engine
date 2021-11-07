#pragma once

#include "Module.h"

#include "Globals.h"
#include "Timer.h"

/* #include "Module.h"
#include "Window.h"
#include "Input.h"
#include "MainScene.h"
#include "Renderer3D.h"
#include "Camera3D.h"
#include "Editor.h"
#include "FileSystem.h"*/

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
	//Timer startupTime;
	//Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	uint64 frameCount = 0;

	float dt;
	int	cappedMs = -1;

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
	inline float& GetDeltaTime() { return dt; }
	int& GetFPS();
	float& GetMaxFPS();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void Load();
	void Save();

};
extern Application* app;