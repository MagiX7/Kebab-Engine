#pragma once
#include "Module.h"
#include "Globals.h"

#include "imgui/imgui.h"

#define BOUNCER_TIME 200

struct PhysBody3D;
class Cube;

class MainScene : public Module
{
public:
	MainScene(Application* app, bool start_enabled = true);
	~MainScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
};
