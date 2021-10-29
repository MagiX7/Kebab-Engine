#include "Application.h"

#include "PanelHierarchy.h"

HierarchyPanel::HierarchyPanel()
{
	active = true;
	scroll = 0;
	goDragging = nullptr;
	optionsPopup = false;

	goDragging = nullptr;
	goClicked = nullptr;
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

		if (ImGui::CollapsingHeader("Game Objects", ImGuiTreeNodeFlags_DefaultOpen))
		{
			int size = app->scene->GetGameObjects().size();
			for (int i = 0; i < size; ++i)
			{
				GameObject* go = app->scene->GetGameObjects()[i];
				DisplayHierarchy(go);
			}
		}
	}
	ImGui::End();
}

void HierarchyPanel::DisplayHierarchy(GameObject* go)
{
	ImGuiTreeNodeFlags flags = 0;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	
	bool opened = ImGui::TreeNodeEx(go->GetName().c_str(), flags);

	////////////////// Current GameObject assignation //////////////////
	if (ImGui::IsItemClicked())
	{
		if (ImGui::IsItemActivated())
			currentGO = go;
		else
			currentGO = nullptr;
	}
	else if (!ImGui::IsAnyItemHovered() && app->input->GetMouseButton(SDL_BUTTON_LEFT))
	{	
		currentGO = nullptr;
	}

	////////////////// Options Popup //////////////////
	if (ImGui::IsItemClicked(1))
	{
		optionsPopup = true;
		goClicked = go;
	}
	else if (!ImGui::IsAnyItemHovered() && app->input->GetMouseButton(SDL_BUTTON_LEFT))
	{
		optionsPopup = false;
		goClicked = nullptr;
	}

	if (optionsPopup && goClicked)
		DisplayGameObjectMenu(goClicked);


	////////////////// Recursively display the hierarchy for its childs //////////////////
	if (opened)
	{
		for (const auto& child : go->GetChilds())
			DisplayHierarchy(child);
		ImGui::TreePop();
	}


	////////////////// Drag and drop //////////////////
	if (ImGui::BeginDragDropSource())
	{
		goDragging = go;

		ImGui::SetDragDropPayload("childs", go, sizeof(GameObject));
		ImGui::Text(go->GetName().c_str());
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

				GameObject* newParent = go->GetParent();

				//newParent->AddChild(goDragging);
				go->AddChild(goDragging);
				goDragging = nullptr;
			}
		}
		ImGui::EndDragDropTarget();
	}
}

void HierarchyPanel::DisplayGameObjectMenu(GameObject* go)
{
	ImGui::OpenPopup(go->GetName().c_str());
	if (ImGui::BeginPopup(go->GetName().c_str()))
	{
		LOG_CONSOLE("POPUP");
		//ImGui::Popupitem
		if (ImGui::Button("Delete"))
		{
			optionsPopup = false;

			app->scene->DeleteGameObject(go);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Empty Child"))
		{
			optionsPopup = false;
			LOG_CONSOLE("EMPTY CHILD POPUP ITEM");

			GameObject* child = new GameObject("Game Object");
			go->AddChild(child);
			child->SetParent(go);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Move up"))
		{
			if (go->GetParent())
			{
				int size = go->GetParent()->GetChilds().size();
				for (int i = 0; i < size; ++i)
				{
					GameObject* current = go->GetParent()->GetChilds()[i];
					if ((current == go) && (i >= 1))
					{
						GameObject* tmp = go->GetParent()->GetChilds()[i - 1];
						go->GetParent()->GetChilds()[i - 1] = go->GetParent()->GetChilds()[i];
						go->GetParent()->GetChilds()[i] = tmp;

						optionsPopup = false;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < app->scene->GetGameObjects().size(); ++i)
				{
					GameObject* current = app->scene->GetGameObjects()[i];
					if (current == go && i >= 1)
					{
						GameObject* tmp = app->scene->GetGameObjects()[i - 1];
						app->scene->GetGameObjects()[i - 1] = app->scene->GetGameObjects()[i];
						app->scene->GetGameObjects()[i] = tmp;

						optionsPopup = false;
						break;
					}
				}
			}

		}
		if (ImGui::Button("Move down"))
		{
			// Childs
			if (go->GetParent())
			{
				int size = go->GetParent()->GetChilds().size();
				for (int i = 0; i < size; ++i)
				{
					GameObject* current = go->GetParent()->GetChilds()[i];
					if ((current == go) && (i <= size - 2))
					{
						GameObject* tmp = go->GetParent()->GetChilds()[i + 1];
						go->GetParent()->GetChilds()[i + 1] = go->GetParent()->GetChilds()[i];
						go->GetParent()->GetChilds()[i] = tmp;

						optionsPopup = false;
						break;
					}
				}
			}
			// Top parents
			else
			{
				int size = app->scene->GetGameObjects().size();
				for (int i = 0; i < size; ++i)
				{
					GameObject* current = app->scene->GetGameObjects()[i];
					if ((current == go) && (i <= size - 2))
					{
						GameObject* tmp = app->scene->GetGameObjects()[i + 1];
						app->scene->GetGameObjects()[i + 1] = app->scene->GetGameObjects()[i];
						app->scene->GetGameObjects()[i] = tmp;

						optionsPopup = false;
						break;
					}
				}
			}
		}
		ImGui::EndPopup();
	}
}
