#include "Application.h"

#include "PanelInspector.h"

#include <vector>

InspectorPanel::InspectorPanel()
{
	this->active = true;
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::OnRender(float dt)
{
	if (ImGui::Begin("Inspector"), &active)
	{
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

		if (app->editor->hierarchyPanel->currentGO)
		{
			std::vector<Component*>::const_iterator it = app->editor->hierarchyPanel->currentGO->GetComponents().begin();
			app->editor->hierarchyPanel->currentGO->GetComponents().size();

			for (; it != app->editor->hierarchyPanel->currentGO->GetComponents().end(); ++it)
			{
				(*it)->DrawOnInspector();
			}
		}

		ImGui::End();
	}
}