#include "Application.h"
#include "PanelConfiguration.h"

#include "Window.h"
#include "Input.h"
#include "Renderer3D.h"

#include "SDL.h"
#include "SDL_cpuinfo.h"
#include "GL/glew.h"
#include "mmgr/mmgr.h"

#include <stdio.h>
#include <string>

ConfigPanel::ConfigPanel()
{
    active = true;

    brightness = 0;
    width = 0;
    height = 0;

    fullscreen = false;
    resizable = false;
    borderless = false;
    fulldesktop = false;

    /*depth = true;
    cullFace = true;
    lighting = true;
    colorMaterial = true;
    texture2D = true;
    showNormals = false;*/

    titleName = "";
    orgName = "";

    i = 0;
    scroll = 0;
}

ConfigPanel::~ConfigPanel()
{
}

void ConfigPanel::OnRender(float dt)
{
    if (ImGui::Begin("Configuration", &active, 0))
    {
        if (ImGui::IsWindowHovered())
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

        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Set Defaults"))
            {

            }
            if (ImGui::MenuItem("Load"))
            {
                app->RequestLoad();
            }
            if (ImGui::MenuItem("Save"))
            {
                app->RequestSave();
            }

            ImGui::EndMenu();
        }

        if (ImGui::CollapsingHeader("Application"))
        {
            // Input Text ===========
            static char tmp[32];
            titleName = app->window->titleName;
            orgName = app->window->orgName;
            sprintf_s(tmp, 32, titleName.c_str());
            if (ImGui::InputText("App Name", tmp, IM_ARRAYSIZE(tmp), ImGuiInputTextFlags_EnterReturnsTrue))
                app->window->SetTitle(tmp);
            sprintf_s(tmp, 32, orgName.c_str());
            if (ImGui::InputText("Organization", tmp, IM_ARRAYSIZE(tmp), ImGuiInputTextFlags_EnterReturnsTrue))
                app->window->orgName = tmp;

            // Slider ============
            static float limFPS = app->GetMaxFPS();
            if (ImGui::SliderFloat("Max FPS", &limFPS, 0.0f, 120.0f, "%.1f"))
                app->SetMaxFPS(limFPS);

            ImGui::Text("Limit Framerate: %.1f", limFPS);

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
                    memCost[y] = memCost[aux];
                    memCost[i] = SDL_GetSystemRAM();
                }
            }
            else
            {
                //fpsLog[i] = ImGui::GetIO().Framerate;
                fpsLog[i] = app->GetDeltaTime();
                ImGuiIO& io = ImGui::GetIO();
                //msLog[i] = 1000.0f / ImGui::GetIO().Framerate;
                msLog[i] = app->GetFPS();
                memCost[i] = SDL_GetSystemRAM();
                i++;
            }
            char title[25];
            //sprintf_s(title, 25, "Framerate: %.1f", ImGui::GetIO().Framerate);
            sprintf_s(title, 25, "Framerate: %i", app->GetFPS());
            ImGui::PlotHistogram("##framerate", fpsLog, IM_ARRAYSIZE(fpsLog), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
            //sprintf_s(title, 25, "Milliseconds: %.3f", 1000.0f / ImGui::GetIO().Framerate);
            sprintf_s(title, 25, "Milliseconds: %.3f", 1000.0f * app->GetDeltaTime());
            ImGui::PlotHistogram("##milliseconds", msLog, IM_ARRAYSIZE(msLog), 0, title, 0.0f, 50.0f, ImVec2(310, 100));
            sprintf_s(title, 25, "Memory Consumption");
            ImGui::PlotHistogram("##memoryconsumption", memCost, IM_ARRAYSIZE(memCost), 0, title, 0.0f, 30000.0f, ImVec2(310, 100));

            sMStats stats = m_getMemoryStatistics();

            ImGui::Text("Total Reported Mem: %i", stats.totalReportedMemory);
            ImGui::Text("Total Actual Mem: %i", stats.totalActualMemory);
            ImGui::Text("Peak Reported Mem: %i", stats.peakReportedMemory);
            ImGui::Text("Peak Actual Mem: %i", stats.peakActualMemory);
            ImGui::Text("Accumulated Reported Mem: %i", stats.accumulatedReportedMemory);
            ImGui::Text("Accumulated Actual Mem: %i", stats.accumulatedActualMemory);
            ImGui::Text("Accumulated Alloc Unit Count: %i", stats.accumulatedAllocUnitCount);
            ImGui::Text("Total Alloc Unit Count: %i", stats.totalAllocUnitCount);
            ImGui::Text("Peak Alloc Unit Count: %i", stats.peakAllocUnitCount);
        }
        if (ImGui::CollapsingHeader("Window"))
        {            
            brightness = app->window->GetBrightness();
            app->window->GetWindowSize(width, height);

            if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
                app->window->SetBrightness(brightness);

            if (ImGui::SliderInt("Width", &width, 0, 1920))
                app->window->SetWindowSize(width, height);

            if (ImGui::SliderInt("Height", &height, 0, 1080))
                app->window->SetWindowSize(width, height);

            ImGui::Text("Refresh rate: %.0f", ImGui::GetIO().Framerate);
            //ImGui::Text("Refresh rate: %.0f", app->GetFPS());

            if (ImGui::BeginTable("Window Config Table", 2))
            {
                fullscreen = app->window->GetFullscreen();
                resizable = app->window->GetResizable();
                borderless = app->window->GetBordered();
                fulldesktop = app->window->GetFullscreenDesktop();

                ImGui::TableNextColumn(); ImGui::Checkbox("Fullscreen", &fullscreen);
                ImGui::TableNextColumn(); ImGui::Checkbox("Resizable", &resizable);
                ImGui::TableNextColumn(); ImGui::Checkbox("Borderless", &borderless);
                ImGui::TableNextColumn(); ImGui::Checkbox("Full Desktop", &fulldesktop);

                app->window->SetFullscreen(fullscreen);
                app->window->SetResizable(resizable);
                app->window->SetFullscreenDesktop(fulldesktop);
                app->window->SetBordered(borderless);

                ImGui::EndTable();
            }
        }
        if (ImGui::CollapsingHeader("File System"))
        {
            char path[100];
            GetModuleFileNameA(NULL, path, 100);
            ImGui::Text("Base Path:");
            ImGui::TextColored({ 200,200,0,255 }, path);
        }
        if (ImGui::CollapsingHeader("Input"))
        {
            ImGui::Text("Mouse Position: "); ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%i, %i", app->input->GetMouseX(), app->input->GetMouseY());
            ImGui::Text("Mouse Motion: "); ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%i,%i", app->input->GetMouseXMotion(), app->input->GetMouseYMotion());
            ImGui::Text("Mouse Wheel: "); ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%i", app->input->GetMouseZ());

            ImGui::Separator();

            ImGui::BeginChild("Input Buffer", ImVec2(0,200));
            ImGui::TextUnformatted(app->input->inputBuf.begin());
            if (app->input->inputBuffScroll)
            {
                ImGui::SetScrollHereY(1.0f);
                app->input->inputBuffScroll = false;
            }
            ImGui::EndChild();
        }
        if (ImGui::CollapsingHeader("Render"))
        {
            if (ImGui::BeginTable("Render Config Table", 2))
            {
                //ImGui::TableNextColumn();
                if (ImGui::Checkbox("Depth", &app->renderer3D->depth))
                    app->renderer3D->SetDepth();

                ImGui::TableNextColumn();

                if (ImGui::Checkbox("Cull Face", &app->renderer3D->cullFace))
                    app->renderer3D->SetCullFace();

                ImGui::TableNextColumn();

                if (ImGui::Checkbox("Lighting", &app->renderer3D->lighting))
                    app->renderer3D->SetLighting();
                
                ImGui::TableNextColumn();
                
                if (ImGui::Checkbox("Color Material", &app->renderer3D->colorMaterial))
                    app->renderer3D->SetColorMaterial();

                ImGui::TableNextColumn();

                if (ImGui::Checkbox("Texture 2D", &app->renderer3D->texture2D))
                    app->renderer3D->SetTexture2D();
                
                ImGui::TableNextColumn();

                //if (ImGui::Checkbox("Draw normals", &app->renderer3D->drawVertexNormals))

                //ImGui::TableNextColumn();

                if (ImGui::Checkbox("Wireframe", &app->renderer3D->wireframe))
                    app->renderer3D->SetWireframe();

                ImGui::TableNextColumn();

                if (ImGui::Checkbox("Draw Colliders", &app->renderer3D->drawAABB))
                    app->renderer3D->SetDrawAABB();

                /*if (ImGui::Checkbox("Frustum Culling", &app->renderer3D->wireframe))
                    app->renderer3D->SetWireframe();*/

                ImGui::EndTable();
            }
        }
        if (ImGui::CollapsingHeader("Hardware"))
        {
            SDL_version compiled;
            SDL_VERSION(&compiled);

            ImGui::Text("SDL ");
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);

            ImGui::Text("OpenGL");
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%s", glGetString(GL_VERSION));
            ImGui::Text("GLEW");
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%s", glewGetString(GLEW_VERSION));
            ImGui::Text("ImGui");
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%s", ImGui::GetVersion());
            
            ImGui::Text("MathGeoLib");
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "1.5.0"); // Not hardcoded because yes. It's the version shown in CMakeLists.txt from MathGeoLib


            ImGui::Separator();

            ImGui::Text("CPUs:");
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%i (Cache: %i kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

            std::string s = "";
            ImGui::Text("Caps:");
            if (SDL_HasAVX()) s += "AVX, ";
            if (SDL_HasAVX2()) s += "AVX2, ";
            if (SDL_HasMMX()) s += "MMX, ";
            if (SDL_HasRDTSC()) s += "RDTSC, ";
            if (SDL_HasSSE()) s += "SSE, ";
            if (SDL_HasSSE2()) s += "SSE2, ";
            if (SDL_HasSSE3()) s += "SSE3, ";
            if (SDL_HasSSE41()) s += "SSE41, ";
            if (SDL_HasSSE42()) s += "SSE42, ";
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%s", s.c_str());

            ImGui::Separator();

            ImGui::Text("GPU:");
            const GLubyte* vendor = glGetString(GL_VENDOR);
            const GLubyte* render = glGetString(GL_RENDERER);
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%s, from %s", render, vendor);

            ImGui::Text("VRAM Budget:");
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%i MB", SDL_GetSystemRAM());
        }

        ImGui::BulletText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
}