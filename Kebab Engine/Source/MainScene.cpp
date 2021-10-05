#include "Application.h"
#include "Input.h"
#include "MainScene.h"
#include "Geometry/Plane.h"
#include "Cube.h"
#include "Sphere.h"

#include "parson.h"


//#include "mmgr/mmgr.h"

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

    //primitive = new Cube({ 0,0,0 }, { 1,1,1 });
    primitive = new Pyramid({ 0,0,0 }, 1.5f, 1.0f);
    //primitive = new MySphere({ 0,0,0 }, 2, 50, 30);

    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(primitive->GetVertices().data(), sizeof(primitive->GetVertices().data()[0]) * primitive->GetVertices().size());

    BufferLayout layout =
    {
        {ShaderDataType::VEC3F, "position"}
    };

    vertexBuffer->SetLayout(layout);
    vertexArray->AddVertexBuffer(*vertexBuffer);

    //indexBuffer = new IndexBuffer(primitive->GetIndices().data() , sizeof(primitive->GetIndices().data()) / sizeof(uint32_t));
    indexBuffer = new IndexBuffer(primitive->GetIndices().data(), sizeof(primitive->GetIndices().data()[0]) * primitive->GetIndices().size());
    vertexArray->SetIndexBuffer(*indexBuffer);

	app->camera->Move(vec3(1.5f, 2.0f, 0.0f));
	app->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Update: draw background
bool MainScene::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) app->RequestSave();
    if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) app->RequestLoad();
    if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
        app->camera->LookAt(vec3(primitive->GetPosition().x, primitive->GetPosition().y, primitive->GetPosition().z));

    vertexArray->Bind();
    //glDrawArrays(GL_TRIANGLES, 0, primitive->GetVertices().size());
    glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    vertexArray->Unbind();

    return true;
}

// Load assets
bool MainScene::CleanUp()
{
    LOG("Unloading scene");
    RELEASE(vertexBuffer);
    RELEASE(vertexArray);

    return true;
}