#include "Application.h"

#include "PanelHierarchy.h"

HierarchyPanel::HierarchyPanel()
{
	active = true;
	scroll = 0;
	goDragging = nullptr;
}

HierarchyPanel::~HierarchyPanel()
{

}

void HierarchyPanel::OnRender(float dt)
{
	if (ImGui::Begin("Hierarchy", &active))
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

		if (ImGui::CollapsingHeader("Game Objects", ImGuiTreeNodeFlags_DefaultOpen))
		{
			int size = app->scene->GetGameObjects().size();
			for (int i = 0; i < size; ++i)
			{
				GameObject* go = app->scene->GetGameObjects()[i];

				if (ImGui::TreeNodeEx(go->GetName().c_str()))
				{
					if (ImGui::IsItemClicked())
						currentGO = go;

					if(go->GetChilds().size() > 0)
						DisplayHierarchy(go);

					ImGui::TreePop();
				}
			}
		}
	}
	ImGui::End();
}

void HierarchyPanel::DisplayHierarchy(GameObject* parentGO)
{
	ImGuiTreeNodeFlags flags = 0;
	flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;;

	for (int i = 0; i < parentGO->GetChilds().size(); ++i)
	{
		const auto& goChild = parentGO->GetChilds()[i];

		if (ImGui::IsItemClicked())
			currentGO = goChild;

		if (ImGui::TreeNodeEx(goChild->GetName().c_str(), flags))
		{
			if (ImGui::IsItemClicked())
				currentGO = goChild;


			DisplayHierarchy(goChild);
			ImGui::TreePop();
		}


		/////// Drag and drop

		if (ImGui::BeginDragDropSource())
		{
			goDragging = goChild;

			ImGui::SetDragDropPayload("childs", goChild, sizeof(GameObject));
			ImGui::Text(goChild->GetName().c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload("childs"))
			{
				if (goDragging)
				{
					GameObject* oldParent = goDragging->GetParent();

					std::vector<GameObject*>::iterator it = oldParent->GetChilds().begin();
					while (*it != goDragging)
						++it;
					oldParent->GetChilds().erase(it);

					GameObject* newParent = goChild->GetParent();

					//newParent->AddChild(goDragging);
					goChild->AddChild(goDragging);
					goDragging = nullptr;
				}
			}
			ImGui::EndDragDropTarget();
		}


	}	
}