#include "Application.h"

#include "mmgr/mmgr.h"

Application::Application()
{
	window = new Window(this);
	input = new Input(this);
	scene = new MainScene(this);
	renderer3D = new Renderer3D(this);
	camera = new Camera3D(this);
	editor = new Editor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(editor);
	
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
		RELEASE(*it);
		it++;
	}
	modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	std::list<Module*>::iterator it = modules.begin();
	
	while (it != modules.end() && ret == true)
	{
		ret = (*it)->Init();
		it++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

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
	dt = (float)msTimer.Read() / 1000.0f;
	msTimer.Start();

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

	//float averageFps = float(frameCount) / (startupTime.Read() / 1000.0f);
	//float secondsSinceStartup = startupTime.Read() / 1000.0f;
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

	std::list<Module*>::iterator it = modules.begin();
	while (it != modules.end())
	{
		(*it)->Load(NULL);
		it++;
	}
}

void Application::Save()
{
	saveReq = false;

	value = json_value_init_object();
	JSON_Object* root = json_value_get_object(value);

	json_object_set_value(root, "App", json_value_init_object());
	json_object_set_number(root, "dt", dt);
	json_object_set_number(root, "max fps", 1000.0f / cappedMs);

	std::list<Module*>::iterator it = modules.begin();
	while (it != modules.end())
	{
		(*it)->Save(root);
		it++;
	}

	json_serialize_to_file_pretty(value, "FILE.json");

	json_value_free(value);
	
	/*char cleanup_command[256];
	sprintf(cleanup_command, "rm -f %s", "FILE.json");
	system(cleanup_command);*/
}

// Call PreUpdate, Update and Draw on all modules
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();

	std::list<Module*>::iterator it = modules.begin();

	while (it != modules.end() && ret == true)
	{
		ret = (*it)->PreUpdate(dt);
		it++;
	}

	it = modules.begin();

	while (it != modules.end() && ret == true)
	{
		ret = (*it)->Update(dt);
		it++;
	}

	it = modules.begin();

	while (it != modules.end() && ret == true)
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

	std::list<Module*>::reverse_iterator it = modules.rbegin();

	while (it != modules.rend())
	{
		ret = (*it)->CleanUp();
		it++;
	}

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
	int res = 1 / dt;
	return res;
}

float& Application::GetMaxFPS()
{
	float ret = cappedMs * 1000;
	return ret;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}