#include "PanelConsole.h"

ConsolePanel::ConsolePanel()
{
}

ConsolePanel::~ConsolePanel()
{
}

bool ConsolePanel::Update(float dt)
{
    if (ImGui::Begin("Console", NULL))
    {

    }
    ImGui::End();
	return true;
}

void ConsolePanel::Draw()
{
}
