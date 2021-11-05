#include "Application.h"
#include "PanelInspector.h"

#include "Input.h"
#include "Editor.h"
#include "MainScene.h"

#include "PanelHierarchy.h"

#include "GameObject.h"
#include "ComponentMesh.h"

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

		if (app->editor->hierarchyPanel->currentGO)
		{
			std::vector<Component*>::const_iterator it = app->editor->hierarchyPanel->currentGO->GetComponents().begin();

			for (; it != app->editor->hierarchyPanel->currentGO->GetComponents().end(); ++it)
			{
				(*it)->DrawOnInspector();
			}
		}

		ImGui::End();
	}
}