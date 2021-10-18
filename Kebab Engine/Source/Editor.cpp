#include "Application.h"
#include "Editor.h"

#include "GL/glew.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "mmgr/mmgr.h"

Editor::Editor(bool startEnabled) : Module(startEnabled)
{
    name = "editor";

    configPanel = new ConfigPanel();
    consolePanel = new ConsolePanel();
    viewportPanel = new ViewportPanel();
    hierarchyPanel = new HierarchyPanel();

    showAboutPanel = false;
    wireframe = true;
}

Editor::~Editor()
{
}

bool Editor::Start()
{
    InitImGui();

	return true;
}

bool Editor::Update(float dt)
{

	return true;
}

bool Editor::Draw(float dt)
{
    //OnImGuiRender(dt);

    //frameBuffer->Bind();

    //glClearColor(0.1f, 0.1f, 0.1f, 1);
    ////glClearDepth(1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    //app->renderer3D->DoDraw();

    //frameBuffer->Unbind();

	return true;
}

bool Editor::CleanUp()
{
    RELEASE(consolePanel);
    RELEASE(viewportPanel);
    RELEASE(hierarchyPanel);
    RELEASE(configPanel);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}

void Editor::InitImGui()
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
    ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->renderer3D->context);
    ImGui_ImplOpenGL3_Init();
}

bool Editor::OnImGuiRender(float dt, FrameBuffer* frameBuffer)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;

    ImGuiWindowFlags flags = 0;
    //flags |= ImGuiWindowFlags_MenuBar;
    //flags |= ImGuiWindowFlags_NoMove;

    static bool showDemoWindow = false;

    ImGui::DockSpaceOverViewport();

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
                consolePanel->active = !consolePanel->active;
            }
            if (ImGui::MenuItem("Configuration"))
            {
                configPanel->active = !configPanel->active;
            }
            if (ImGui::MenuItem("Hierarchy"))
            {
                hierarchyPanel->active = !hierarchyPanel->active;
            }
            if (ImGui::Checkbox(" Wireframe", &wireframe))
            {
                //wireframe = !wireframe;
                app->renderer3D->SetWireframe();
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
                showAboutPanel = !showAboutPanel;
                scroll = 0;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
    viewportPanel->OnRender(frameBuffer);
    ImGui::PopStyleVar();

    if (showDemoWindow)
    {
        bool demoShouldClose = true;
        ImGui::ShowDemoWindow(&demoShouldClose);
        if (!demoShouldClose) showDemoWindow = false;
    }

    if (showAboutPanel) ShowAboutPanel();

    if (hierarchyPanel->active) hierarchyPanel->OnRender(dt);
    if (consolePanel->active) consolePanel->OnRender(dt);
    if (configPanel->active) configPanel->OnRender(dt);

    ImGui::EndFrame();



    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    //frameBuffer->Unbind();

    return true;
}


void Editor::ShowAboutPanel()
{
    ImGui::Begin("About Kebab Engine", &showAboutPanel);
    ImGui::SetWindowSize({ 800,600 });

    if (ImGui::GetMousePos().x > ImGui::GetWindowPos().x && ImGui::GetMousePos().x < ImGui::GetWindowPos().x + ImGui::GetWindowWidth() &&
        ImGui::GetMousePos().y > ImGui::GetWindowPos().y && ImGui::GetMousePos().y < ImGui::GetWindowPos().y + ImGui::GetWindowHeight())
    {
        if (app->input->GetMouseZ() < 0)
            scroll -= app->input->GetMouseZ() * 5;
        if (app->input->GetMouseZ() > 0)
            scroll -= app->input->GetMouseZ() * 5;
        if (scroll <= 0) scroll = 0;
        else if (scroll >= ImGui::GetScrollMaxY()) scroll = ImGui::GetScrollMaxY();
    }

    ImGui::SetScrollY(scroll);

    ImGui::Text("Kebab Engine v0.1");
    ImGui::NewLine();
    ImGui::Text("Description here.");
    ImGui::NewLine();

    ImGui::Text("Created by Carlos Megia and David Gonzalez.");
    ImGui::NewLine();

    ImGui::Text("Third party libraries used:");

    SDL_version ver;
    SDL_VERSION(&ver);
    ImGui::BulletText("SDL Version: %d.%d.%d", ver.major, ver.minor, ver.patch);
    ImGui::BulletText("OpenGL %s", glGetString(GL_VERSION));
    ImGui::BulletText("GLEW %s", glewGetString(GLEW_VERSION));
    ImGui::BulletText("ImGui %s", ImGui::GetVersion());
    ImGui::BulletText("MathGeoLib");
    
    ImGui::NewLine();

    ImGui::Text("License: ");
    ImGui::Text("MIT License");
    ImGui::Text("\n");
    ImGui::Text("Copyright(c) 2021 David González and Carlos Megia");
    ImGui::Text("\n");
    ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
    ImGui::Text("of this softwareand associated documentation files(the 'Software'), to deal");
    ImGui::Text("in the Software without restriction, including without limitation the rights");
    ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and /or sell");
    ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
    ImGui::Text("furnished to do so, subject to the following conditions :");
    ImGui::Text("\n");
    ImGui::Text("The above copyright noticeand this permission notice shall be included in all");
    ImGui::Text("copies or substantial portions of the Software.");
    ImGui::Text("\n");
    ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
    ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
    ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
    ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
    ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
    ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
    ImGui::Text("SOFTWARE.");

    // TODO: Read file with JSON and print it

    ImGui::End();
}