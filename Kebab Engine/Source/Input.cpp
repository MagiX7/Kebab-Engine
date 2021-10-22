#include "Application.h"
#include "Input.h"

#include "MeshLoader.h"

#include "imgui/imgui_impl_sdl.h"

#include "mmgr/mmgr.h"

#define MAX_KEYS 300

Input::Input(bool startEnabled) : Module(startEnabled)
{
	name = "input";

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
}

// Destructor
Input::~Input()
{
	RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool Input::Init(JSON_Object* root)
{
	LOG_CONSOLE("Init SDL input event system");
  
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG_CONSOLE("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
bool Input::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

	mouseX /= SCREEN_SIZE;
	mouseY /= SCREEN_SIZE;
	mouseZ = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouseButtons[i] == KEY_IDLE)
				mouseButtons[i] = KEY_DOWN;
			else
				mouseButtons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouseButtons[i] == KEY_REPEAT || mouseButtons[i] == KEY_DOWN)
				mouseButtons[i] = KEY_UP;
			else
				mouseButtons[i] = KEY_IDLE;
		}
	}

	mouseXMotion = mouseYMotion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			{
				mouseZ = e.wheel.y;
				break;
			}

			case SDL_MOUSEMOTION:
			{
				mouseX = e.motion.x / SCREEN_SIZE;
				mouseY = e.motion.y / SCREEN_SIZE;

				mouseXMotion = e.motion.xrel / SCREEN_SIZE;
				mouseYMotion = e.motion.yrel / SCREEN_SIZE;
				break;
			}
			case SDL_QUIT:
			{
				quit = true;
				break;
			}
			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					app->renderer3D->OnResize(e.window.data1, e.window.data2);
				break;
			}
			case SDL_DROPFILE:
			{
				std::string droppedFileDir = e.drop.file;
				//char* droppedFiledir = e.drop.file;
				// Shows directory of dropped file
				SDL_ShowSimpleMessageBox(
					SDL_MESSAGEBOX_INFORMATION,
					"File dropped on window",
					droppedFileDir.c_str(),
					app->window->window
				);

				int end = droppedFileDir.find(".");
				std::string extension = droppedFileDir.substr(end + 1);

				int start = droppedFileDir.find_last_of("\\") + 1;
				std::string name = droppedFileDir.substr(start, end - start);
				

				// Convert extension to lowercase
				std::for_each(extension.begin(), extension.end(), [](char& c)
				{
					c = ::tolower(c);
				});

				if(extension == "fbx")
					app->renderer3D->Submit(MeshLoader::GetInstance()->LoadModel(droppedFileDir));
				else if (extension == "dds" || extension == "png")
				{
					// TODO: Check the current game object and attach the texture to it
				}

				droppedFileDir.clear();
				extension.clear();
				//SDL_free(droppedFileDir);    // Free droppedFiledir memory
				break;
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return false;

	return true;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG_CONSOLE("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}