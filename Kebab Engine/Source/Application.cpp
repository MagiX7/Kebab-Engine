#include "Application.h"

Application::Application()
{
	window = new Window(this);
	input = new Input(this);
	scene = new MainScene(this);
	renderer3D = new Renderer3D(this);
	camera = new Camera3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(renderer3D);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene);
}

Application::~Application()
{
	std::list<Module*>::iterator it = listModules.begin();

	while (it != listModules.end())
	{
		delete (*it);
		it++;
	}
}

bool Application::Init()
{
	bool ret = true;

	std::list<Module*>::iterator it = listModules.begin();
	
	while (it != listModules.end() && ret == true)
	{
		ret = (*it)->Init();
		it++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	it = listModules.begin();

	while (it != listModules.end() && ret == true)
	{
		ret = (*it)->Start();
		it++;
	}
	
	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)msTimer.Read() / 1000.0f;
	msTimer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and Draw on all modules
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();

	std::list<Module*>::iterator it = listModules.begin();

	while (it != listModules.end() && ret == true)
	{
		ret = (*it)->PreUpdate(dt);
		it++;
	}

	it = listModules.begin();

	while (it != listModules.end() && ret == true)
	{
		ret = (*it)->Update(dt);
		it++;
	}

	it = listModules.begin();

	while (it != listModules.end() && ret == true)
	{
		ret = (*it)->Draw(dt);
		it++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::list<Module*>::iterator it = listModules.begin();

	while (it != listModules.end() && ret == true)
	{
		ret = (*it)->CleanUp();
		it++;
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	listModules.push_back(mod);
}