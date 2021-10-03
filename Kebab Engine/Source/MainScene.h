#pragma once
#include "Module.h"
#include "Globals.h"

#include "VertexArray.h"

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
	VertexBuffer* vertexBuffer;
	VertexArray* vertexArray;
	IndexBuffer* indexBuffer;
};
