#include "Globals.h"
#include "Application.h"
#include "Window.h"

Window::Window(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	window = NULL;
	screenSurface = NULL;

	name = "window";
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Init()
{
	LOG("Init SDL window & surface");
	LOG("Creating SDL window & surface\n");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
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
	LOG("Destroying SDL window and quitting all SDL systems");

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
}

void Window::Save(JSON_Object* root)
{
	json_object_set_value(root, name.c_str(), json_value_init_object());
	JSON_Object* winObj = json_object_get_object(root, name.c_str());

	json_object_set_number(winObj, "width", width);
	json_object_set_number(winObj, "height", height);
	json_object_set_number(winObj, "brightness", brightness);
	json_object_set_boolean(winObj, "fullwindow", fullscreen);
	json_object_set_boolean(winObj, "fulldesktop", fulldesktop);
	json_object_set_boolean(winObj, "resizable", resizable);
	json_object_set_boolean(winObj, "bordered", borderless);
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
	app->renderer3D->OnResize(w, h);
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