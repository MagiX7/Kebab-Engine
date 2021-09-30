#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class Window : public Module
{
public:

	Window(Application* app, bool startEnabled = true);

	// Destructor
	virtual ~Window();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	JSON_Object* Save(JSON_Object* root) override;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;

private:
	int width;
	int height;

	JSON_Value* value;

};

#endif // __ModuleWindow_H__