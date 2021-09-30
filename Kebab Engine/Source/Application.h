#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "Window.h"
#include "Input.h"
#include "MainScene.h"
#include "Renderer3D.h"
#include "Camera3D.h"
#include "Editor.h"

#include "parson.h"

#include <list>

class Application
{
public:
	Window* window;
	Input* input;
	MainScene* scene;
	Renderer3D* renderer3D;
	Camera3D* camera;
	Editor* editor;

private:

	Timer	msTimer;
	float   pastDt = 63355;
	float	dt;
	std::list<Module*> listModules;

	bool saveReq;
	bool loadReq;
	JSON_Value* value;

public:

	Application();
	~Application();

	bool Init();
	bool Update();
	bool CleanUp();

	void RequestSave();
	void RequestLoad();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void PrintCommitsInfo(const char* username, const char* repo);

	void Load();
	void Save();

};