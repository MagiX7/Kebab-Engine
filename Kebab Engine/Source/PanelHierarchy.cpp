#include "PanelHierarchy.h"

HierarchyPanel::HierarchyPanel()
{
	active = true;
	scroll = 0;
}

HierarchyPanel::~HierarchyPanel()
{

}

void HierarchyPanel::OnRender(float dt)
{
	if (ImGui::Begin("Hierarchy", &active))
	{

	}
	ImGui::End();
}