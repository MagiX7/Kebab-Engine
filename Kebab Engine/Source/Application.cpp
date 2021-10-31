#include "Application.h"

#include "mmgr/mmgr.h"

Application::Application()
{
	window = new Window();
	input = new Input();
	scene = new MainScene();
	renderer3D = new Renderer3D();
	camera = new Camera3D();
	editor = new Editor();
	fileSystem = new FileSystem();
	textures = new TextureManager();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(textures);
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
		RELEASE(*it);
		it++;
	}
	modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	std::list<Module*>::iterator it = modules.begin();

	value = json_parse_file("config.json");
	if (!value)
	{
		LOG_CONSOLE("Could not load or there is no file to load config.json");
		ret = false;
	}
	else
	{
		JSON_Object* root = json_value_get_object(value);
		JSON_Object* appObj = json_object_get_object(root, "App");
		dt = json_object_get_number(appObj, "dt");
		cappedMs = 1000.0f / json_object_get_number(appObj, "max fps");

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

	JSON_Object* root = json_value_get_object(value);

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

	value = json_value_init_object();
	JSON_Object* root = json_value_get_object(value);

	json_object_set_value(root, "App", json_value_init_object());
	JSON_Object* appObj = json_object_get_object(root, "App");
	json_object_set_number(appObj, "dt", dt);
	json_object_set_number(appObj, "max fps", GetMaxFPS());

	std::list<Module*>::iterator it = modules.begin();
	while (it != modules.end())
	{
		(*it)->Save(root);
		it++;
	}

	json_serialize_to_file_pretty(value, "config.json");

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

	//MeshLoader::GetInstance()->CleanUp();

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
	float ret = 1000 / cappedMs;
	return ret;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}