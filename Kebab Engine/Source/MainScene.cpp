#include "Application.h"
#include "Input.h"
#include "MainScene.h"
#include "Cube.h"
#include "Sphere.h"
#include "Plane.h"

#include "parson.h"


#include "mmgr/mmgr.h"

#include <iostream>

MainScene::MainScene(bool startEnabled) : Module(startEnabled)
{
    name = "mainscene";
}

MainScene::~MainScene()
{}

// Load assets
bool MainScene::Start()
{
	LOG("Loading assets");
    //LOG("Creating Scene\n");
	bool ret = true;

    //primitive = new KebabCube({ 0,0,0 }, { 1,1,1 });
    //primitive = new KebabPyramid({ 0,0,0 }, 5.f, 4.0f);
    //primitive = new KebabSphere({ 0,0,0 }, 2, 50, 30);
    //primitive = new KebabPlane({ -5,0,0 }, { 10,5 });
    //primitive = new KebabGeometry();
    /*primitive = meshLoader->LoadMesh("Assets/3D Models/warrior.fbx");
    app->renderer3D->Submit(primitive);*/

    app->renderer3D->Submit(MeshLoader::GetInstance()->LoadMesh("Assets/3D Models/warrior.fbx"));

    /*app->renderer3D->Submit(new KebabPyramid({ 0,0,0 }, 5.f, 4.0f));
    app->renderer3D->Submit(new KebabCube({ 5.5f,0,-3 }, { 5,5,5 }));*/

	app->camera->Move(vec3(1.5f, 2.0f, 0.0f));
	app->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Update: draw background
bool MainScene::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) app->RequestSave();
    if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) app->RequestLoad();

    //primitive->Draw();

    /*vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    vertexArray->Unbind();*/

    return true;
}

// Load assets
bool MainScene::CleanUp()
{
    LOG("Unloading scene");

    return true;
}