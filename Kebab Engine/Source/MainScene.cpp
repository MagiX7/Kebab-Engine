#include "Application.h"
#include "MainScene.h"

#include "Input.h"
#include "Renderer3D.h"
#include "Editor.h"

#include "PanelHierarchy.h"

#include "GameObject.h"
#include "MeshLoader.h"

#include "Cube.h"
#include "Sphere.h"

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
    root = new GameObject("Scene");
    gameObjects.push_back(root);

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
    if (!go->GetParent())
    {
        go->SetParent(root);
        root->AddChild(go);
    }
    //gameObjects.push_back(go);
}

void MainScene::DeleteGameObject(GameObject* go)
{
    std::vector<GameObject*>::iterator it;

    for (it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        if (*it == go)
        {
            root->GetChilds().erase(it);
            root->GetChilds().shrink_to_fit();
            //gameObjects.erase(it);
            //gameObjects.shrink_to_fit();

            app->renderer3D->EraseGameObject(go);
            app->editor->hierarchyPanel->currentGO = nullptr;
            delete(go);
            go = nullptr;
            break;
        }
    }
}

std::vector<GameObject*>& MainScene::GetGameObjects()
{
    return root->GetChilds();
}

void MainScene::DeleteAllGameObjects()
{
    for (auto& go : root->GetChilds()/*gameObjects*/)
    {
        delete go;
        go = nullptr;
    }
    root->GetChilds().clear();
    //gameObjects.clear();
}

GameObject* MainScene::GetGameObjectByUuid(int uuid)
{
    for (const auto& go : root->GetChilds()/*gameObjects*/)
        if (go->GetUuid() == uuid) return go;

    return nullptr;
}
