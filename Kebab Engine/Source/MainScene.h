#pragma once
#include "Module.h"
#include "Globals.h"

#include "Buffer.h"

#define BOUNCER_TIME 200

class MainScene : public Module
{
public:
	MainScene(Application* app, bool startEnabled = true);
	~MainScene();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	VertexBuffer* vbo;

};
