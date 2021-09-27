#include "Application.h"
#include "Editor.h"

#include "GL/glew.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

Editor::Editor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

Editor::~Editor()
{
}

bool Editor::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	showCase = false;

	return true;
}

bool Editor::Update(float dt)
{
    LOG("EDITOR UPDATE");

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;

    ImGuiWindowFlags flags = 0;
    //flags |= ImGuiWindowFlags_MenuBar;
    //flags |= ImGuiWindowFlags_NoMove;

    //ImGui::SetWindowPos(p);
    //ImGui::SetWindowSize(s);

    static bool showDemoWindow = false;

    //ImGui::SetWindowCollapsed(true);

    if (ImGui::Begin("Console", NULL))
    {

    }
    ImGui::End();

    if (ImGui::Begin("Configuration", NULL, flags))
    {
        if (ImGui::CollapsingHeader("Application"))
        {
            // Input Text ===========
            static char tmp[32];
            sprintf_s(tmp, 32, "Kebab Engine");
            ImGui::InputText("App Name", tmp, IM_ARRAYSIZE(tmp));
            sprintf_s(tmp, 32, "UPC CITM");
            ImGui::InputText("Organization", tmp, IM_ARRAYSIZE(tmp));

            // Slider ============

            static float limFPS = 0;
            ImGui::SliderFloat("Max FPS", &limFPS, 0.0f, 120.0f, "%.1f");

            ImGui::BulletText("Limit Framerate: %.1f", limFPS);

            // Histogram ============
            if (i == (MAX_IT_HIST - 1))
            {
                for (int y = 0; y < i; y++)
                {
                    int aux = y + 1;
                    fpsLog[y] = fpsLog[aux];
                    fpsLog[i] = ImGui::GetIO().Framerate;
                    msLog[y] = msLog[aux];
                    msLog[i] = 1000.0f / ImGui::GetIO().Framerate;
                }
            }
            else
            {
                fpsLog[i] = ImGui::GetIO().Framerate;
                msLog[i] = 1000.0f / ImGui::GetIO().Framerate;
                i++;
            }
            char title[25];
            sprintf_s(title, 25, "Framerate %.1f", ImGui::GetIO().Framerate);
            ImGui::PlotHistogram("##framerate", fpsLog, IM_ARRAYSIZE(fpsLog), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
            sprintf_s(title, 25, "Milliseconds %.3f", 1000.0f / ImGui::GetIO().Framerate);
            ImGui::PlotHistogram("##milliseconds", msLog, IM_ARRAYSIZE(msLog), 0, title, 0.0f, 50.0f, ImVec2(310, 100));
        }
        if (ImGui::CollapsingHeader("Window"))
        {

        }
        if (ImGui::CollapsingHeader("File System"))
        {

        }
        if (ImGui::CollapsingHeader("Input"))
        {

        }
        if (ImGui::CollapsingHeader("Hardware"))
        {

        }

        ImGui::BulletText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                ImGui::EndMenu();
                return false;
            }
            ImGui::EndMenu();
        }
        /*if (ImGui::BeginMenu("Examples"))
        {
            if (ImGui::MenuItem("Demo"))
            {
                showDemoWindow = !showDemoWindow;
            }
            ImGui::EndMenu();
        }*/

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Gui Demo"))
            {
                //showCase = !showCase;
                showDemoWindow = !showDemoWindow;
            }
            if (ImGui::MenuItem("Documentation"))
            {

            }
            if (ImGui::MenuItem("Download Latest"))
            {

            }
            if (ImGui::MenuItem("Report a bug"))
            {

            }
            if (ImGui::MenuItem("About"))
            {

            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    

    if (showDemoWindow)
    {
        bool demoShouldClose = true;
        ImGui::ShowDemoWindow(&demoShouldClose);
        if (!demoShouldClose) showDemoWindow = false;
    }


	/*if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Gui Demo"))
		{
			showCase = !showCase;
		}
		if (ImGui::MenuItem("Documentation"))
		{

		}
		if (ImGui::MenuItem("Download Latest"))
		{

		}
		if (ImGui::MenuItem("Report a bug"))
		{

		}
		if (ImGui::MenuItem("About"))
		{

		}

		ImGui::EndMenu();
	}*/

	return true;
}

bool Editor::Draw(float dt)
{
    LOG("EDITOR DRAW");

    ImVec4 clearColor = ImVec4(0.1, 0.1, 0.1, 0.1);

    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //SDL_GL_SwapWindow(App->window->window); -> Done in Render

	return true;
}

bool Editor::CleanUp()
{
	return true;
}