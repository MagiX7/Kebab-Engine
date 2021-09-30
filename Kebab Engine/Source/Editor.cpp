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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

    configPanel.windowConfig = App->window;
    configPanel.renderConfig = App->renderer3D;

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
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Console"))
            {
                consolePanel.active = !consolePanel.active;
            }
            if (ImGui::MenuItem("Configuration"))
            {
                configPanel.active = !configPanel.active;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Gui Demo"))
            {
                showDemoWindow = !showDemoWindow;
            }
            if (ImGui::MenuItem("Documentation"))
            {
                ShellExecute(NULL, "open", "https://github.com/MagiX7/Kebab-Engine", NULL, NULL, SW_SHOWNORMAL);
            }
            if (ImGui::MenuItem("Download Latest"))
            {
                ShellExecute(NULL, "open", "https://github.com/MagiX7/Kebab-Engine/archive/refs/heads/main.zip", NULL, NULL, SW_SHOWNORMAL);
            }
            if (ImGui::MenuItem("Report a bug"))
            {
                ShellExecute(NULL, "open", "https://github.com/MagiX7/Kebab-Engine/issues", NULL, NULL, SW_SHOWNORMAL);
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

    if (consolePanel.active) consolePanel.Update(dt);
    if (configPanel.active) configPanel.Update(dt);

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

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

	return true;
}

bool Editor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}