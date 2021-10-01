#include "Application.h"
#include "Input.h"
#include "MainScene.h"
//#include "Primitive.h"
//#include "MathGeoLib/src/Geometry/Plane.h"
#include "Geometry/Plane.h"

#include "parson/src/parson.h"

#include "GL/glew.h"

//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_sdl.h"
//#include "imgui/imgui_impl_opengl3.h"

//#include "glut.h"

#include <iostream>

MainScene::MainScene(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

MainScene::~MainScene()
{}

// Load assets
bool MainScene::Start()
{
	LOG("Loading assets");
    app->editor->consolePanel.AddLog("Creating Scene\n");
	bool ret = true;

    vbo = new VertexBuffer();

	App->camera->Move(vec3(1.5f, 2.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

    //// Setup Dear ImGui context
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    ////io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    ////io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    //// Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ////ImGui::StyleColorsClassic();

    //// Setup Platform/Renderer backends
    //ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    //ImGui_ImplOpenGL3_Init();

	return ret;
}

// Update: draw background
bool MainScene::Update(float dt)
{
    if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) App->RequestSave();

    // CUBE
    float vertices2[] = {
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
    };

    vbo->SetData(vertices2, sizeof(vertices2));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    vbo->Bind();
    glDrawArrays(GL_TRIANGLES, vertices2[0], 12 * 3);

    return true;
}

// Load assets
bool MainScene::CleanUp()
{
    LOG("Unloading scene");

    return true;
}