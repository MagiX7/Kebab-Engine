#include "Application.h"
#include "MainScene.h"
//#include "Primitive.h"
//#include "MathGeoLib/src/Geometry/Plane.h"
#include "Geometry/Plane.h"

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
	LOG("Loading Intro assets");
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
    // Start the Dear ImGui frame
    //ImGui_ImplOpenGL3_NewFrame();
    //ImGui_ImplSDL2_NewFrame();
    //ImGui::NewFrame();

    //static float f = 0.0f;
    //static int counter = 0;

    //// Menu ============================================

    //ImGuiWindowFlags flags = 0;
    ////flags |= ImGuiWindowFlags_MenuBar;
    ////flags |= ImGuiWindowFlags_NoMove;

    ////ImGui::SetWindowPos(p);
    ////ImGui::SetWindowSize(s);

    //static bool showDemoWindow = false;

    //ImGui::SetWindowCollapsed(true);

    //if (ImGui::Begin("Options..", NULL, flags))
    //{
    //    if (ImGui::BeginChild("Framerate"))
    //    {
    //        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //        ImGui::EndChild();
    //    }
    //}
    //ImGui::End();

    //if (ImGui::BeginMainMenuBar())
    //{
    //    if (ImGui::BeginMenu("File"))
    //    {
    //        if (ImGui::MenuItem("Exit"))
    //        {
    //            ImGui::EndMenu();
    //            return false;
    //        }
    //        ImGui::EndMenu();
    //    }
    //    if (ImGui::BeginMenu("Examples"))
    //    {
    //        if (ImGui::MenuItem("Demo"))
    //        {
    //            showDemoWindow = !showDemoWindow;
    //        }
    //        ImGui::EndMenu();
    //    }
    //    ImGui::EndMainMenuBar();
    //}

    //if (showDemoWindow)
    //{
    //    bool demoShouldClose = true;
    //    ImGui::ShowDemoWindow(&demoShouldClose);
    //    if (!demoShouldClose) showDemoWindow = false;
    //}

    // Rendering ===============================

    //glClear(GL_COLOR_BUFFER_BIT);

    //Plane p(0, 1, 0, 0);
    //math::Plane p(math::vec(0, 1, 0), 0);

    //p.Render();

    // ImGui Stuff
    //ImVec4 clearColor = ImVec4(0.1, 0.1, 0.1, 0.1);

    //ImGui::Render();
    //ImGuiIO& io = ImGui::GetIO();
    //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    //glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    ////glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ////SDL_GL_SwapWindow(App->window->window); -> Done in Render



    // TRIANGLE
    /*float vertices[3 * 3] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f, 0.5f, 0.0f
    };*/

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
    LOG("Unloading Intro scene");

    return true;
}