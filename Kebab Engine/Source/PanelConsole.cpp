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

        if (app->input->GetMouseZ() < 0)
            scroll -= app->input->GetMouseZ() * 5;
        if (app->input->GetMouseZ() > 0)
            scroll -= app->input->GetMouseZ() * 5;

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
