#include "Application.h"
#include "Input.h"
#include "MainScene.h"
#include "Cube.h"
#include "Sphere.h"
//#include "Plane.h"

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
	LOG_CONSOLE("Loading assets");

	bool ret = true;

    app->renderer3D->Submit(MeshLoader::GetInstance()->LoadModel("Assets/Resources/Baker House.fbx"));

	return ret;
}

// Update: draw background
bool MainScene::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) app->RequestSave();
    if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) app->RequestLoad();

    return true;
}

// Load assets
bool MainScene::CleanUp()
{
    LOG_CONSOLE("Unloading scene");

    for (auto& go : gameObjects)
    {
        delete(go);
    }
    gameObjects.clear();

    return true;
}

void MainScene::AddGameObject(GameObject* go)
{
    gameObjects.push_back(go);
}

void MainScene::DeleteGameObject(GameObject* go)
{
    std::vector<GameObject*>::iterator it;

    for (it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        if (*it == go)
        {
            gameObjects.erase(it);
            gameObjects.shrink_to_fit();

            app->renderer3D->EraseGameObject(go);
            app->editor->hierarchyPanel->currentGO = nullptr;
            delete(go);
            go = nullptr;
            break;
        }
    }
}