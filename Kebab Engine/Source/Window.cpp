#include "Application.h"
#include "Window.h"

#include "Globals.h"

#include "mmgr/mmgr.h"

Window::Window(bool startEnabled) : Module(startEnabled)
{
	window = NULL;
	screenSurface = NULL;

	name = "window";

	brightness = 1;
	fullscreen = false;
	resizable = true;
	borderless = false;
	fulldesktop = false;
	titleName = "";
	orgName = "UPC CITM";
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Init(JSON_Object* root)
{
	SDL_version ver;
	SDL_VERSION(&ver);
	std::string s = "Initializing SDL. Version: " + std::to_string(ver.major) + '.' + std::to_string(ver.minor) + '.' + std::to_string(ver.patch);
	LOG_CONSOLE(s.c_str());

	LOG_CONSOLE("Init SDL window & surface");
  
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_CONSOLE("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Load(root);
		/*JSON_Object* winObj = json_object_get_object(root, name.c_str());
		width = json_object_get_number(winObj, "width");
		height = json_object_get_number(winObj, "height");
		brightness = json_object_get_number(winObj, "brightness");
		fullscreen = json_object_get_boolean(winObj, "fullscreen");
		fulldesktop = json_object_get_boolean(winObj, "fulldesktop");
		resizable = json_object_get_boolean(winObj, "resizable");
		borderless = json_object_get_boolean(winObj, "bordered");*/

		//Create window
		/*width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;*/
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(fulldesktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		if (titleName.size() > 0) SetTitle(titleName.c_str());
		else titleName = TITLE;

		if(window == NULL)
		{
			LOG_CONSOLE("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool Window::CleanUp()
{
	LOG_CONSOLE("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void Window::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
	titleName = title;
}

void Window::Save(JSON_Object* root)
{
	json_object_set_value(root, name.c_str(), json_value_init_object());
	JSON_Object* winObj = json_object_get_object(root, name.c_str());

	json_object_set_number(winObj, "width", width);
	json_object_set_number(winObj, "height", height);
	json_object_set_number(winObj, "brightness", brightness);
	json_object_set_boolean(winObj, "fullscreen", fullscreen);
	json_object_set_boolean(winObj, "fulldesktop", fulldesktop);
	json_object_set_boolean(winObj, "resizable", resizable);
	json_object_set_boolean(winObj, "bordered", borderless);
	json_object_set_string(winObj, "title", titleName.c_str());
	json_object_set_string(winObj, "organization", orgName.c_str());
}

void Window::Load(JSON_Object* root)
{
	json_object_set_value(root, name.c_str(), json_object_get_value(root, name.c_str()));
	JSON_Object* winObj = json_object_get_object(root, name.c_str());

	width = json_object_get_number(winObj, "width");
	height = json_object_get_number(winObj, "height");
	brightness = json_object_get_number(winObj, "brightness");
	fullscreen = json_object_get_boolean(winObj, "fullscreen");
	fulldesktop = json_object_get_boolean(winObj, "fulldesktop");
	resizable = json_object_get_boolean(winObj, "resizable");
	borderless = json_object_get_boolean(winObj, "bordered");
	titleName = json_object_get_string(winObj, "title");
	orgName = json_object_get_string(winObj, "organization");
}

void Window::SetBrightness(float value)
{
	SDL_SetWindowBrightness(window, value);
	brightness = value;
}

void Window::SetWindowSize(float w, float h)
{
	SDL_SetWindowSize(window, w, h);
	width = w;
	height = h;
}

void Window::SetFullscreen(bool value)
{
	SDL_SetWindowFullscreen(window, value ? SDL_WINDOW_FULLSCREEN : 0);
	fullscreen = value;
}

void Window::SetFullscreenDesktop(bool value)
{
	SDL_SetWindowFullscreen(window, value ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	fulldesktop = value;
}

void Window::SetResizable(bool value)
{
	SDL_SetWindowResizable(window, value ? SDL_TRUE : SDL_FALSE);
	resizable = value;
}

void Window::SetBordered(bool value)
{
	SDL_SetWindowBordered(window, value ? SDL_FALSE : SDL_TRUE);
	borderless = value;
}

SDL_Rect Window::GetViewport()
{
	return { 0,0,width,height };
}