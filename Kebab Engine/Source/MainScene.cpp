#include "Application.h"
#include "MainScene.h"

#include "Input.h"
#include "Camera3D.h"
#include "Renderer3D.h"
#include "Editor.h"

#include "PanelHierarchy.h"

#include "MeshLoader.h"
#include "QdTree.h"

#include "KbCube.h"
#include "KbSphere.h"

#include "parson.h"

#include <iostream>
#include <queue>

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
    ComponentTransform* trans = (ComponentTransform*)goCam->GetComponent(ComponentType::TRANSFORM);
    trans->SetTranslation(float3(-5, 5, 2));
    //camera->SetCameraPosition({ -5,5,2 });
    camera->Look({ 0,0,0 });

    ComponentTransform* tr = (ComponentTransform*)goCam->GetComponent(ComponentType::TRANSFORM);
    tr->SetTranslation({ -5,5,2 });

    //tr->SetTranslation(camera->GetCameraPosition());
    goCam->AddComponent(camera);

    AddGameObject(goCam);
    app->camera->SetGameCamera(camera);

    rootQT = new QdTree();
    vec min = vec(-50, -10, -50);
    vec max = vec(50, 50, 50);
    AABB aabbAux;
    aabbAux.minPoint = min;
    aabbAux.maxPoint = max;
    rootQT->Create(aabbAux);

    avril = MeshLoader::GetInstance()->LoadModelCustomFormat("Avril.kbmodel");
    app->renderer3D->Submit(avril);
    //GameObject* bh = MeshLoader::GetInstance()->LoadModel("Assets/Resources/Baker House.fbx");
    //app->renderer3D->Submit(bh);

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
    }
    if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
    {
        GameObject* bh = MeshLoader::GetInstance()->LoadModel("Assets/Resources/Baker House.fbx");
        app->renderer3D->Submit(bh);
    }
    if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
    {
        //avril = MeshLoader::GetInstance()->LoadModelCustomFormat("Avril.kbmodel");
        //avril = MeshLoader::GetInstance()->LoadModel("Assets/Resources/Avril.fbx");
        app->renderer3D->Submit(avril);
    }
    if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
    {
        MeshLoader::GetInstance()->SaveModelCustomFormat(avril);
        //app->renderer3D->Submit(bh);
    }
    if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
        rootQT->Recalculate();

    rootQT->Intersect(&app->camera->editorCam->frustum);

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
    go->SetParent(root);
    //gameObjects.push_back(go);
}

void MainScene::DeleteGameObject(GameObject* go)
{
    std::vector<GameObject*>::iterator it;

    rootQT->Remove(go);
    rootQT->Recalculate();

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

void MainScene::EraseGameObject(GameObject* go)
{
    std::vector<GameObject*>::iterator it = root->GetChilds().begin();

    for (; it != root->GetChilds().end(); ++it)
    {
        if ((*it) == go)
        {
            root->GetChilds().erase(it);
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
    std::queue<GameObject*> q;
    for (const auto& go : root->GetChilds())
        q.push(go);

    while (!q.empty())
    {
        GameObject* curr = q.front();
        q.pop();

        if (curr->GetUuid() == uuid) return curr;

        for (const auto& child : curr->GetChilds())
            q.push(child);
    }


    return nullptr;
}
