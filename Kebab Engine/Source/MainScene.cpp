#include "Application.h"
#include "MainScene.h"

#include "Input.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "Editor.h"

#include "PanelHierarchy.h"

#include "MeshLoader.h"
#include "QdTree.h"

#include "Cube.h"
#include "Sphere.h"

#include "parson.h"

#include <iostream>

#include "mmgr/mmgr.h"


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

    GameObject* goCam = new GameObject("Main Camera");
    camera = new ComponentCamera(goCam, CameraType::GAME);
    camera->SetCameraPosition({ -5,5,2 });
    camera->Look({ 0,0,0 });

    ComponentTransform* tr = (ComponentTransform*)goCam->GetComponent(ComponentType::TRANSFORM);
    //tr->SetTranslation({ -5,5,2 });

    //tr->SetTranslation(camera->GetCameraPosition());
    goCam->AddComponent(camera);

    AddGameObject(goCam);
    app->camera->SetGameCamera(camera);


    avril = MeshLoader::GetInstance()->LoadModelCustomFormat("Avril.kbmodel");
    app->renderer3D->Submit(avril);
    //GameObject* bh = MeshLoader::GetInstance()->LoadModel("Assets/Resources/Baker House.fbx");
    //app->renderer3D->Submit(bh);

    rootQT = new QdTree();
    vec min = vec(-10, -10, -10);
    vec max = vec(100, 100, 100);
    AABB aabbAux;
    aabbAux.minPoint = min;
    aabbAux.maxPoint = max;
    rootQT->Create(aabbAux);

    rootQT->Insert(avril);
    //rootQT->Recalculate();

	return ret;
}

// Update: draw background
bool MainScene::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) app->RequestSave();
    if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) app->RequestLoad();

    static GameObject* a = nullptr;

    if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
    {
        GameObject* test = MeshLoader::GetInstance()->LoadModelCustomFormat("Baker House.kbmodel");
        app->renderer3D->Submit(test);
        rootQT->Insert(test);
        //rootQT->Recalculate();

    }
    if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
    {
        GameObject* bh = MeshLoader::GetInstance()->LoadModel("Assets/Resources/Baker House.fbx");
        app->renderer3D->Submit(bh);
    }
    if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
    {
        GameObject* a = MeshLoader::GetInstance()->LoadModelCustomFormat("Avril.kbmodel");
        app->renderer3D->Submit(a);
    }
    if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
    {
        MeshLoader::GetInstance()->SaveModelCustomFormat(avril);
        //app->renderer3D->Submit(bh);
    }

    return true;
}

// Load assets
bool MainScene::CleanUp()
{
    LOG_CONSOLE("Unloading scene");

    for (auto& go : root->GetChilds())
    {
        delete(go);
    }
    root->GetChilds().clear();
    delete root;

    return true;
}

void MainScene::AddGameObject(GameObject* go)
{
    root->AddChild(go);
    //gameObjects.push_back(go);
}

void MainScene::DeleteGameObject(GameObject* go)
{
    std::vector<GameObject*>::iterator it;

    for (it = root->GetChilds().begin(); it != root->GetChilds().end(); ++it)
    {
        if (*it == go)
        {
            root->GetChilds().erase(it);
            root->GetChilds().shrink_to_fit();

            app->renderer3D->EraseGameObject(go);
            app->editor->hierarchyPanel->currentGO = nullptr;
            delete(go);
            go = nullptr;
            break;
        }
    }
}

void MainScene::DeleteAllGameObjects()
{
    for (auto& go : root->GetChilds())
    {
        delete go;
        go = nullptr;
    }

    root->GetChilds().clear();
}

GameObject* MainScene::GetGameObjectByUuid(int uuid)
{
    for (const auto& go : root->GetChilds())
        if (go->GetUuid() == uuid) return go;

    return nullptr;
}
