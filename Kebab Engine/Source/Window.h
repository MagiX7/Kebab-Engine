#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class Window : public Module
{
public:

	Window(bool startEnabled = true);
	virtual ~Window();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void Save(JSON_Object* root) override;
	void Load(JSON_Object* root) override;

	void SetBrightness(float value);
	void SetWindowSize(float w, float h);
	void SetFullscreen(bool value);
	void SetFullscreenDesktop(bool value);
	void SetResizable(bool value);
	void SetBordered(bool value);

	inline float& GetBrightness() { return brightness; }
	inline void GetWindowSize(int& w, int& h) { w = width; h = height; }

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;

private:
	int width;
	int height;

	float brightness = 1;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fulldesktop;

	JSON_Value* value;
};

#endif // __ModuleWindow_H__