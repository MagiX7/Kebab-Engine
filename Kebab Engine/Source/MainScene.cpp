#include "Application.h"
#include "Input.h"
#include "MainScene.h"
#include "Geometry/Plane.h"

#include "parson.h"

#include "GL/glew.h"

#include "mmgr/mmgr.h"

#include <iostream>

MainScene::MainScene(Application* app, bool startEnabled) : Module(app, startEnabled)
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

    /*float vertices[] = {
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
         0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
         0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f,-0.5f, 0.5f
    };*/

    float vertices[] = {
        // front
        -0.5, -0.5,  0.5,
         0.5, -0.5,  0.5,
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        // back
        -0.5, -0.5, -0.5,
         0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
        -0.5,  0.5, -0.5
    };
    uint32_t indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices, sizeof(vertices));

    BufferLayout layout =
    {
        {ShaderDataType::VEC3F, "position"}
    };

    vertexBuffer->SetLayout(layout);
    vertexArray->AddVertexBuffer(*vertexBuffer);

    indexBuffer = new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
    vertexArray->SetIndexBuffer(*indexBuffer);

	App->camera->Move(vec3(1.5f, 2.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Update: draw background
bool MainScene::Update(float dt)
{
    if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) App->RequestSave();

    //vbo->Bind();
    vertexArray->Bind();
    //glDrawArrays(GL_TRIANGLES, , 36 * 3);
    //glDrawArrays(GL_TRIANGLES, vertexArray->GetVertexBuffers().data()[0], )
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