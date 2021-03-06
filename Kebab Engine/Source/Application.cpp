#include "Application.h"

#include "Window.h"
#include "Input.h"
#include "MainScene.h"
#include "Renderer3D.h"
#include "Camera3D.h"
#include "Editor.h"
#include "FileSystem.h"

#include "MeshLoader.h"
#include "TextureLoader.h"
#include "ResourceManager.h"

#include "Parser.h"

#include "mmgr/mmgr.h"

Application::Application()
{
	window = new Window();
	input = new Input();
	renderer3D = new Renderer3D();
	camera = new Camera3D();
	editor = new Editor();
	scene = new MainScene();
	fileSystem = new FileSystem();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(editor);
	AddModule(fileSystem);

	// Scenes
	AddModule(scene);

	AddModule(renderer3D);

	saveReq = false;
	loadReq = false;
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator it = modules.rbegin();
	while (it != modules.rend())
	{
		delete(*it);
		*it = nullptr;
		it++;
	}
	modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	std::list<Module*>::iterator it = modules.begin();
	
	value = Parser::ParseFile("Settings/config.json");
	if (!value)
	{
		LOG_CONSOLE("Could not load or there is no file to load config.json");
		ret = false;
	}
	else
	{
		JSON_Object* root = Parser::GetObjectByValue(value);
		JSON_Object* appObj = Parser::GetObjectByName(root, "App");
		appDt = Parser::GetNumberByObject(appObj, "dt");
		cappedMs = 1000.0f / Parser::GetNumberByObject(appObj, "max fps");

		while (it != modules.end() && ret == true)
		{
			ret = (*it)->Init(root);
			it++;
		}
	}
	// After all Init calls we call Start() in all modules
	LOG_CONSOLE("Application Start --------------");
  
	it = modules.begin();

	while (it != modules.end() && ret == true)
	{
		ret = (*it)->Start();
		it++;
	}

	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
	appDt = (float)msTimer.Read() / 1000.0f;
	msTimer.Start();

	if (editor->GetSceneState() == SceneState::PLAY)
	{
		runtimeDt = (float)runtimeTimer.Read() / 1000.0f;
		lastRuntimeDt = runtimeDt;

		runtimeFrameCount++;
		totalRuntimeFrameCount++;

		runtimeTimer.Start();
	}
	else if (editor->GetSceneState() == SceneState::STEP_ONE_FRAME)
	{
		runtimeDt = lastRuntimeDt;
		runtimeFrameCount++;
		totalRuntimeFrameCount++;
		
		editor->SetSceneState(SceneState::PAUSE);
	}
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (saveReq) Save();
	if (loadReq) Load();

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	uint32 lastFrameMs = msTimer.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	if ((cappedMs > 0) && (lastFrameMs < cappedMs))
	{
		SDL_Delay(cappedMs - lastFrameMs);
	}
}

void Application::Load()
{
	loadReq = false;

	JSON_Object* root = Parser::GetObjectByValue(value);

	std::list<Module*>::iterator it = modules.begin();
	while (it != modules.end())
	{
		(*it)->Load(root);
		it++;
	}
}

void Application::Save()
{
	saveReq = false;

	value = Parser::InitValue();
	JSON_Object* root = Parser::GetObjectByValue(value);
	Parser::SetObjectValue(root, "App");
	JSON_Object* appObj = Parser::GetObjectByName(root, "App");
	Parser::SetObjectNumber(appObj, "dt", appDt);
	Parser::SetObjectNumber(appObj, "max fps", GetMaxFPS());

	std::list<Module*>::iterator it = modules.begin();
	while (it != modules.end())
	{
		(*it)->Save(root);
		it++;
	}

	Parser::GenerateFile(value, "Settings/config.json");

	Parser::FreeValue(value);
}

// Call PreUpdate, Update and Draw on all modules
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();

	std::list<Module*>::iterator it = modules.begin();

	while (it != modules.end() && ret == true)
	{
		ret = (*it)->PreUpdate(appDt);
		it++;
	}

	it = modules.begin();

	while (it != modules.end() && ret == true)
	{
		ret = (*it)->Update(appDt);
		it++;
	}

	it = modules.begin();

	while (it != modules.end() && ret == true)
	{
		ret = (*it)->Draw(appDt);
		it++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::list<Module*>::reverse_iterator it = modules.rbegin();

	while (it != modules.rend())
	{
		ret = (*it)->CleanUp();
		it++;
	}

	ResourceManager::GetInstance()->CleanUp();
	MeshLoader::GetInstance()->CleanUp();
	TextureLoader::GetInstance()->CleanUp();

	return ret;
}

void Application::RequestSave()
{
	saveReq = true;
}

void Application::RequestLoad()
{
	loadReq = true;
}

void Application::SetMaxFPS(int fps)
{
	if (fps != 0)
		cappedMs = 1000 / fps;
}

int& Application::GetFPS()
{
	int res = 1 / appDt;
	return res;
}

float& Application::GetMaxFPS()
{
	float ret = 1000 / cappedMs;
	return ret;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}