#include "PanelConfiguration.h"

#include <stdio.h>

ConfigPanel::ConfigPanel()
{
}

ConfigPanel::~ConfigPanel()
{
}

bool ConfigPanel::Update(float dt)
{
    if (ImGui::Begin("Configuration", NULL, 0))
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

	return true;
}

void ConfigPanel::Draw()
{
}
