#include "Application.h"

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
	if (ImGui::Begin("Hierarchy"/*, &active*/))
	{
		if (ImGui::CollapsingHeader("Game Objects"))
		{
			int size = app->scene->GetGameObjects().size();
			for (int i = 0; i < size; ++i)
			{
				DisplayHierarchy(app->scene->GetGameObjects()[i]);
			}
		}
	}
	ImGui::End();
}

void HierarchyPanel::DisplayHierarchy(GameObject* gameObject)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(gameObject->GetName().c_str()))
	{
		for (int i = 0; i < gameObject->GetChilds().size(); ++i)
		{
			const auto& goChild = gameObject->GetChilds()[i];

			if (ImGui::TreeNodeEx(goChild->GetName().c_str(), flags))
			{
				if (goChild->GetChilds().size() > 0)
				{
					DisplayHierarchy(goChild->GetChilds()[0]);
					
					// Support drag and drop too

				}

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("_TREENODE", goChild, sizeof(GameObject));
					ImGui::Text("Name: %s", goChild->GetName().c_str());
					ImGui::EndDragDropSource();
				}

				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}