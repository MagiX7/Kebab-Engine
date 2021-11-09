#include "Application.h"
#include "FileSystem.h"
#include "Editor.h"

#include "Window.h"
#include "Renderer3D.h"
#include "MainScene.h"
#include "Input.h"

#include "Buffer.h"
#include "MeshLoader.h"

#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelHierarchy.h"
#include "PanelInspector.h"
#include "PanelViewport.h"
#include "PanelAssets.h"


#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include "mmgr/mmgr.h"

Editor::Editor(bool startEnabled) : Module(startEnabled)
{
    name = "editor";

    configPanel = new ConfigPanel();
    consolePanel = new ConsolePanel();
    viewportPanel = new ViewportPanel();
    hierarchyPanel = new HierarchyPanel();
    inspectorPanel = new InspectorPanel();
    assetsPanel = new AssetsPanel();

    showAboutPanel = false;
    showWindows = true;
    closeApp = false;
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
    if (closeApp) return false;
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
    delete(consolePanel);
    consolePanel = nullptr;
    delete(viewportPanel);
    viewportPanel = nullptr;
    delete(hierarchyPanel);
    hierarchyPanel = nullptr;
    delete(configPanel);
    configPanel = nullptr;
    delete(inspectorPanel);
    inspectorPanel = nullptr;
    delete(assetsPanel);
    assetsPanel = nullptr;

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
    //ImGui::StyleColorsDark();
    SetImGuiStyle();
    //ImGui::PushStyleColor(ImGuiCol_DockingPreview, { 0.1f,0.1f,0.1f,1 });
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
            if (ImGui::MenuItem("Save Scene", "Crtl + S"))
            {
                SaveScene();
            }
            if (ImGui::MenuItem("Open Scene"))
            {
                LoadScene();
            }
            if (ImGui::MenuItem("Exit"))
            {
                closeApp = true;
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("GameObject"))
        {
            if (ImGui::MenuItem("Cube"))
                app->renderer3D->Submit(MeshLoader::GetInstance()->LoadKbGeometry(KbGeometryType::CUBE));

            if (ImGui::MenuItem("Pyramid"))
                app->renderer3D->Submit(MeshLoader::GetInstance()->LoadKbGeometry(KbGeometryType::PYRAMID));
            
            if(ImGui::MenuItem("Plane"))
                app->renderer3D->Submit(MeshLoader::GetInstance()->LoadKbGeometry(KbGeometryType::PLANE));
            
            if (ImGui::MenuItem("Sphere"))
                app->renderer3D->Submit(MeshLoader::GetInstance()->LoadKbGeometry(KbGeometryType::SPHERE));

            if (ImGui::MenuItem("Empty GameObject"))
            {
                GameObject* go = new GameObject("Empty Game Object");
                app->scene->AddGameObject(go);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Console window"))
            {
                consolePanel->active = !consolePanel->active;
            }
            if (ImGui::MenuItem("Configuration window"))
            {
                configPanel->active = !configPanel->active;
            }
            if (ImGui::MenuItem("Hierarchy window"))
            {
                hierarchyPanel->active = !hierarchyPanel->active;
            }
            if (ImGui::MenuItem("Inspector window"))
            {
                inspectorPanel->active = !inspectorPanel->active;
            }
            if (ImGui::MenuItem("Assets window"))
            {
                assetsPanel->active = !assetsPanel->active;
            }
            ImGui::Checkbox("Show Editor Windows", &showWindows);
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

    SimulationControl();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
    if(frameBuffer)
        viewportPanel->OnRender(frameBuffer);
    ImGui::PopStyleVar();

    if (showDemoWindow)
    {
        bool demoShouldClose = true;
        ImGui::ShowDemoWindow(&demoShouldClose);
        if (!demoShouldClose) showDemoWindow = false;
    }

    if (showWindows)
    {
        if (showAboutPanel) ShowAboutPanel();

        if (consolePanel->active) consolePanel->OnRender(dt);
        if (configPanel->active) configPanel->OnRender(dt);
        if (hierarchyPanel->active) hierarchyPanel->OnRender(dt);
        if (inspectorPanel->active) inspectorPanel->OnRender(dt);
        if (assetsPanel->active) assetsPanel->OnRender(dt);
    }

    ImGui::EndFrame();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backupCurrentWindow = SDL_GL_GetCurrentWindow();
        SDL_GLContext backupCurrentContext = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
    }

    //frameBuffer->Unbind();

    return true;
}

void Editor::SaveScene()
{
    sceneValue = Parser::InitValue();
    JSON_Object* root = Parser::GetObjectByValue(sceneValue);

    JSON_Value* arrValue = Parser::InitArray();
    JSON_Array* gosArray = Parser::GetArrayByValue(arrValue);

    Parser::DotSetObjectValue(root, "Scene.GameObjects", arrValue);

    for (const auto& go : app->scene->GetGameObjects())
        go->Save(gosArray);

    size_t size = Parser::GetSerializationSize(sceneValue);
    char* buffer = new char[size];
    json_serialize_to_buffer(sceneValue, buffer, size);
    if (app->fileSystem->Save("Assets/Scenes/Scene.kbscene", buffer, size) > 0)
        LOG_CONSOLE("Saved successfully");
    delete[] buffer;

    Parser::GenerateFile(sceneValue, "Assets/Scenes/JSON/Scene.json");
    Parser::FreeValue(sceneValue);
}

void Editor::LoadScene()
{
    app->scene->DeleteAllGameObjects();
    app->renderer3D->EraseAllGameObjects();
    hierarchyPanel->currentGO = nullptr;
    
    char* buffer;
    if(app->fileSystem->Load("Assets/Scenes/Scene.kbscene", &buffer) > 0)
    {
        sceneValue = json_parse_string(buffer);
        JSON_Object* sceneObj = Parser::GetObjectByValue(sceneValue);

        JSON_Array* gosArray = json_object_dotget_array(sceneObj, "Scene.GameObjects");

        int numGos = json_array_get_count(gosArray);
        for (int i = 0; i < numGos; ++i)
        {
            JSON_Object* obj = json_array_get_object(gosArray, i);
            const char* name = Parser::GetStringByObject(obj, "name");
            int uuid = Parser::GetNumberByObject(obj, "uuid");
            GameObject* go = new GameObject(name, uuid);

            go->Load(obj);

            if(!go->GetParent())
                app->scene->AddGameObject(go);

            if (go->GetComponent(ComponentType::MESH) && go->GetComponent(ComponentType::MATERIAL))
                app->renderer3D->Submit(go);
        }
    }
}

void Editor::ShowAboutPanel()
{
    ImGui::Begin("About Kebab Engine", &showAboutPanel);
    ImGui::SetWindowSize({ 800,600 });

    if (app->input->GetMouseZ() < 0)
    {
        if (app->input->GetMouseZ() < 0)
        {
            scroll = ImGui::GetScrollY();
            scroll -= app->input->GetMouseZ();
            ImGui::SetScrollY(scroll);
        }
        if (app->input->GetMouseZ() > 0)
        {
            scroll = ImGui::GetScrollY();
            scroll -= app->input->GetMouseZ();
            ImGui::SetScrollY(scroll);
        }
        if (scroll <= 0) scroll = 0;
        else if (scroll >= ImGui::GetScrollMaxY()) scroll = ImGui::GetScrollMaxY();
    }

    ImGui::Text("Kebab Engine v0.1");
    ImGui::NewLine();
    ImGui::Text("This is an 3D Game Engine made for the subject Game Engines at CITM - UPC Terrassa (2021)");
    ImGui::Text("The objective is to create a useful 3D Engine that fits all the basic aspects to create a 3D game");
    ImGui::Text("and to learn how to do and works an Engine.");
    ImGui::NewLine();

    ImGui::Text("Created by Carlos Megia and David Gonzalez.");
    ImGui::NewLine();

    ImGui::Text("Third party libraries used:");

    SDL_version ver;
    SDL_VERSION(&ver);
    ImGui::BulletText("SDL %d.%d.%d", ver.major, ver.minor, ver.patch);
    ImGui::BulletText("OpenGL %s", glGetString(GL_VERSION));
    ImGui::BulletText("GLEW %s", glewGetString(GLEW_VERSION));
    ImGui::BulletText("ImGui %s", ImGui::GetVersion());
    ImGui::BulletText("MathGeoLib 1.5.0"); // Not hardcoded because yes. It's the version shown in CMakeLists.txt from MathGeoLib

    ImGui::NewLine();

    ImGui::Text("License: ");
    ImGui::Text("MIT License");
    ImGui::Text("\n");
    ImGui::Text("Copyright(c) 2021 David Gonzalez and Carlos Megia");
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

    ImGui::End();
}

void Editor::SimulationControl()
{
    if (ImGui::Begin("SimulationControl", NULL, ImGuiWindowFlags_NoTitleBar))
    {
        if (ImGui::Button("Play"));

        ImGui::SameLine();

        if (ImGui::Button("Stop"));

        ImGui::SameLine();

        if (ImGui::Button("Reset"));

        ImGui::End();
    }
}

void Editor::SetImGuiStyle()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    // -- UI Style Settings --
    style->WindowPadding = ImVec2(5, 5);
    style->WindowRounding = 3.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 5.0f;
    style->ItemSpacing = ImVec2(15, 8);
    style->ItemInnerSpacing = ImVec2(10, 8);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;

    style->TabRounding = style->FrameRounding;
    //style->ChildRounding = 4.0f;
    style->PopupRounding = 3.0f;
    style->TabBorderSize = 0.1f;


    // UI Style Colors
    colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.34f, 0.33f, 0.4f, 1.0f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.0f);
    colors[ImGuiCol_Border] = ImVec4(0.8f, 0.8f, 0.83f, 0.88f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1f, 0.09f, 0.12f, 1.0f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4f, 0.4f, 0.4f, 0.7f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 0.75f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.8f, 0.8f, 0.8f, 0.8f);

    colors[ImGuiCol_CheckMark] = ImVec4(0.96f, 0.96f, 0.97f, 0.9f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.65f, 0.65f, 0.65f, 0.75f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.4f, 0.8f, 0.9f, 1.0f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.0f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.0f);

    colors[ImGuiCol_PlotLines] = ImVec4(0.6f, 0.6f, 0.9f, 1.f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.7f, 0.6f, 1.0f);
    colors[ImGuiCol_PlotHistogram] = colors[ImGuiCol_PlotLines];
    colors[ImGuiCol_PlotHistogramHovered] = colors[ImGuiCol_PlotLinesHovered];

    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.78f, 0.5f, 0.5f, 0.85f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.0f, 0, 0, 0);

    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.13f, 0.13f, 0.21f, 0.8f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.17f, 0.17f, 0.25f, 0.9f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.7f, 1.0f);

    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.14f, 0.13f, 0.17f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.0f);

    // Frames
    colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.8f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.3f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.0f);

    // Titles
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.12f, 0.18f, 0.8f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.18f, 0.23f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.07f, 0.09f, 1.0f);


    colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_Separator] = ImVec4(0.65f, 0.65f, 0.7f, 0.88f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1f, 0.4f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.1f, 0.4f, 0.75f, 1.0f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.65f, 0.65f, 0.95f, 0.95f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);

    colors[ImGuiCol_DockingPreview] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.8f);


    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.44f, 0.42f, 0.42f, 0.5f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.45f, 0.46f, 0.9f);
    colors[ImGuiCol_TabActive] = ImVec4(0.65f, 0.65f, 0.65f, 0.65f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.392f, 0.369f, 0.376f, 0.5f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.392f, 0.369f, 0.376f, 0.50f);
}