#pragma once
#include "Module.h"
#include "Globals.h"

#include "MeshLoader.h"

#include "VertexArray.h"

#include "Cube.h"
#include "Pyramid.h"

#define BOUNCER_TIME 200

class MainScene : public Module
{
public:
	MainScene(bool startEnabled = true);
	~MainScene();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	VertexBuffer* vertexBuffer;
	VertexArray* vertexArray;
	IndexBuffer* indexBuffer;

	KebabGeometry* primitive;


	MeshLoader* meshLoader;
};
