#include "Application.h"

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

	PrintCommitsInfo("magix7", "Kebab-Engine");

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

void Application::PrintCommitsInfo(const char* username, const char* repo)
{
	JSON_Value* root_value;
	JSON_Array* commits;
	JSON_Object* commit;
	size_t i;

	char curl_command[512];
	char cleanup_command[256];
	char output_filename[] = "commits.json";

	/* It ain't pretty, but it's what it is */
	sprintf(curl_command,
		"curl -s \"https://api.github.com/repos/%s/%s/commits\" > %s",
		username, repo, output_filename);
	sprintf(cleanup_command, "rm -f %s", output_filename);
	system(curl_command);

	/* parsing json and validating output */
	root_value = json_parse_file(output_filename);
	if (json_value_get_type(root_value) != JSONArray) {
		system(cleanup_command);
		return;
	}

	/* getting array from root value and printing commit info */
	commits = json_value_get_array(root_value);
	printf("%-10.10s %-10.10s %s\n", "Date", "SHA", "Author");
	for (i = 0; i < json_array_get_count(commits); i++) {
		commit = json_array_get_object(commits, i);
		printf("%.10s %.10s %s\n",
			json_object_dotget_string(commit, "commit.author.date"),
			json_object_get_string(commit, "sha"),
			json_object_dotget_string(commit, "commit.author.name"));
	}

	/* cleanup code */
	json_value_free(root_value);
	system(cleanup_command);
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