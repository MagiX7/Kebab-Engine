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

bool ConsolePanel::Update(float dt)
{
    if (ImGui::Begin("Console", &active))
    {
        ImGui::TextUnformatted(buf.begin());

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

        if (scrollMax)
        {
            scrollMax = false;
            scroll = ImGui::GetWindowHeight();
        }
        ImGui::SetScrollY(scroll);
    }
    ImGui::End();

	return true;
}

void ConsolePanel::Draw()
{
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
