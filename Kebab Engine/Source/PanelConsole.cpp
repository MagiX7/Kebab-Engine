#include "PanelConsole.h"

ConsolePanel::ConsolePanel()
{
    ClearConsole();
}

ConsolePanel::~ConsolePanel()
{
    ClearConsole();
}

bool ConsolePanel::Update(float dt)
{
    if (ImGui::Begin("Console", NULL))
    {
        ImGui::TextUnformatted(Buf.begin(), "\n");
        ImGui::SetScrollY(ImGui::GetWindowHeight());
    }
    ImGui::End();
	return true;
}

void ConsolePanel::Draw()
{
}

void ConsolePanel::AddLog(const char* fmt)
{
    va_list args;
    va_start(args, fmt);
    Buf.appendfv(fmt, args);
    va_end(args);
}

void ConsolePanel::ClearConsole()
{
    Buf.clear();
}
