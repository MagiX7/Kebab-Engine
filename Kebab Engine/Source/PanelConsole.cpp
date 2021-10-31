#include "PanelConsole.h"
#include "Application.h"

#include "mmgr/mmgr.h"

ConsolePanel::ConsolePanel()
{
    active = true;
    scroll = 0;
    scrollMax = false;
    ClearConsole();
}

ConsolePanel::~ConsolePanel()
{
    ClearConsole();
}

void ConsolePanel::OnRender(float dt)
{
    if (ImGui::Begin("Console", &active))
    {
        ImGui::TextUnformatted(buf.begin());

        if (ImGui::IsWindowHovered())
        {
            if (app->input->GetMouseZ() < 0)
            {
                scroll = ImGui::GetScrollY();
                scroll -= app->input->GetMouseZ() * 2;
                ImGui::SetScrollY(scroll);
            }
            if (app->input->GetMouseZ() > 0)
            {
                scroll = ImGui::GetScrollY();
                scroll -= app->input->GetMouseZ() * 2;
                ImGui::SetScrollY(scroll);
            }
            if (scroll <= 0) scroll = 0;
            else if (scroll >= ImGui::GetScrollMaxY()) scroll = ImGui::GetScrollMaxY();
        }

        if (scrollMax)
        {
            ImGui::SetScrollHereY(1.0f);
            scrollMax = false;
        }
    }
    ImGui::End();
}

void ConsolePanel::AddLog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    buf.appendfv(fmt, args);
    buf.appendfv("\n", args);
    va_end(args);

    scrollMax = true;
}

void ConsolePanel::ClearConsole()
{
    buf.clear();
}
