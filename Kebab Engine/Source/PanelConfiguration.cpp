#include "Application.h"
#include "Window.h"

#include "PanelConfiguration.h"

#include "SDL.h"
#include "SDL_cpuinfo.h"
#include "GL/glew.h"

#include <stdio.h>
#include <string>

ConfigPanel::ConfigPanel()
{
    active = true;

    brightness = 0;
    width = 0;
    height = 0;

    depth = false;
    cullFace = false;
    lighting = false;
    colorMaterial = false;
    texture2D = false;
}

ConfigPanel::~ConfigPanel()
{
}

bool ConfigPanel::Update(float dt)
{
    if (ImGui::Begin("Configuration", NULL, 0))
    {
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Set Defaults"))
            {

            }
            if (ImGui::MenuItem("Load"))
            {

            }
            if (ImGui::MenuItem("Save"))
            {

            }

            ImGui::EndMenu();
        }

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
            if (ImGui::SliderFloat("Max FPS", &limFPS, 0.0f, 120.0f, "%.1f"))
            {
                app->SetMaxFPS(limFPS);
            }

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
        }
        if (ImGui::CollapsingHeader("Window"))
        {            
            brightness = SDL_GetWindowBrightness(windowConfig->window);
            SDL_GetWindowSize(windowConfig->window, &width, &height);
            if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
            {
                app->window->SetBrightness(brightness);
            }
            if (ImGui::SliderInt("Width", &width, 0, 1920))
            {
                app->window->SetWindowSize(width, height);
                //renderConfig->OnResize(width, height); -> Done in SetWindowSize()
            }           
            if (ImGui::SliderInt("Height", &height, 0, 1080))
            {
                app->window->SetWindowSize(width, height);
            }

            ImGui::Text("Refresh rate: %.0f", ImGui::GetIO().Framerate);

            if (ImGui::BeginTable("Window Config Table", 2))
            {
                ImGui::TableNextColumn(); ImGui::Checkbox("Fullscreen", &fullscreen);
                ImGui::TableNextColumn(); ImGui::Checkbox("Resizable", &resizable);
                ImGui::TableNextColumn(); ImGui::Checkbox("Borderless", &borderless);
                ImGui::TableNextColumn(); ImGui::Checkbox("Full Desktop", &fulldesktop);

                app->window->SetFullscreen(fullscreen);
                app->window->SetResizable(resizable);
                app->window->SetFullscreenDesktop(fulldesktop);
                app->window->SetBordered(borderless);
                //SDL_SetWindowFullscreen(windowConfig->window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
                //SDL_SetWindowResizable(windowConfig->window, resizable ? SDL_TRUE : SDL_FALSE);
                //SDL_SetWindowBordered(windowConfig->window, borderless ? SDL_FALSE : SDL_TRUE);
                //SDL_SetWindowFullscreen(windowConfig->window, fulldesktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

                ImGui::EndTable();
            }
        }
        if (ImGui::CollapsingHeader("File System"))
        {

        }
        if (ImGui::CollapsingHeader("Input"))
        {

        }
        if (ImGui::CollapsingHeader("Render"))
        {
            if (ImGui::BeginTable("Render Config Table", 2))
            {
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Depth", &depth))
                {
                    depth ? glDisable(GL_DEPTH_TEST) : glEnable(GL_DEPTH_TEST);
                    LOG("-- GL_DEPTH_TEST -- set to %d", depth);
                }
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Cull", &cullFace))
                {
                    cullFace ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);
                    LOG("-- GL_CULL_FACE -- set to %d", cullFace);
                }
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Lighting", &lighting))
                {
                    cullFace ? glDisable(GL_LIGHTING) : glEnable(GL_LIGHTING);
                    LOG("-- GL_LIGHTING -- set to %d", lighting);
                }
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Color Material", &colorMaterial))
                {
                    colorMaterial ? glDisable(GL_COLOR_MATERIAL) : glEnable(GL_COLOR_MATERIAL);
                    LOG("-- GL_COLOR_MATERIAL -- set to %d", colorMaterial);
                }
                ImGui::TableNextColumn();
                if (ImGui::Checkbox("Texture 2D", &texture2D))
                {
                    texture2D ? glDisable(GL_TEXTURE_2D) : glEnable(GL_TEXTURE_2D);
                    LOG("-- GL_TEXTURE_2D -- set to %d", texture2D);
                }
                ImGui::EndTable();
            }
        }
        if (ImGui::CollapsingHeader("Hardware"))
        {
            SDL_version compiled;
            SDL_VERSION(&compiled);

            ImGui::Text("SDL Version:");
            ImGui::SameLine(); ImGui::TextColored({ 200,200,0,255 }, "%d.%d.%d", compiled.major, compiled.minor, compiled.patch);

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

	return true;
}

void ConfigPanel::Draw()
{
}
