#include "Application.h"
#include "PanelHierarchy.h"

#include "Input.h"
#include "MainScene.h"
#include "Renderer3D.h"

#include "GameObject.h"
#include "ComponentMaterial.h"

HierarchyPanel::HierarchyPanel()
{
	active = true;
	scroll = 0;
	goDragging = nullptr;
	optionsPopup = false;
	parentingPopup = false;

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

		if (ImGui::CollapsingHeader("Game Objects", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (int i = 0; i < app->scene->GetGameObjects().size(); ++i)
			{
				GameObject* go = app->scene->GetGameObjects()[i];
				DisplayHierarchy(go);
			}
		}
	}
	ImGui::End();
}

void HierarchyPanel::SetCurrent(GameObject* go)
{
	currentGO = go;
	selectedFromViewport = !selectedFromViewport;
}

void HierarchyPanel::DisplayHierarchy(GameObject* go)
{
	ImGuiTreeNodeFlags flags = 0;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	if (go->GetChilds().size() <= 0) flags |= ImGuiTreeNodeFlags_Leaf;
	
	ImGui::PushID(go->GetUuid());
	bool opened = ImGui::TreeNodeEx(go->GetName().c_str(), flags);

	////////////////// Current GameObject assignation //////////////////
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		if (ImGui::IsItemActivated())
			currentGO = go;
		else
			currentGO = nullptr;
	}
	else if (!ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_LEFT))
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


	////////////////// Recursively display the hierarchy for its childs //////////////////
	if (opened)
	{
		for (int i = 0; i < go->GetChilds().size(); ++i)
			DisplayHierarchy(go->GetChilds()[i]);

		ImGui::TreePop();
	}

	// This goes after because if we delete a child, when the recursive call comes, it crashes because go was deleted
	if (optionsPopup && goClicked == go)
		DisplayGameObjectMenu(goClicked);


	////////////////// Drag and drop //////////////////
	if (ImGui::BeginDragDropSource())
	{
		// TODO: Something weird happens when u drag and drop the last child
		/*if (goDragging != go->GetParent())
		{
			goDragging = go;
		}*/

		if (goDragging && goDragging != go && goDragging->GetParent() == go->GetParent())
			goDragging = go->GetChilds()[go->GetChilds().size()];
		if(!goDragging)
			goDragging = go;

		ImGui::SetDragDropPayload("TREENODE", goDragging, sizeof(GameObject));
		ImGui::Text(goDragging->GetName().c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("TREENODE"))
		{
			if (goDragging)
			{
				GameObject* oldParent = goDragging->GetParent();

				if (oldParent != app->scene->GetRoot())
				{
					std::vector<GameObject*>::iterator it = oldParent->GetChilds().begin();
					while (*it != goDragging)
						++it;
					oldParent->GetChilds().erase(it);
				}
				else if(oldParent == app->scene->GetRoot())
				{
					goDragging->UnParent();
					app->scene->EraseGameObject(goDragging);
				}
				go->AddChild(goDragging);
				goDragging->SetParent(go);

				goDragging = nullptr;	
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::PopID();
}

void HierarchyPanel::DisplayGameObjectMenu(GameObject* go)
{
	ImGui::OpenPopup(go->GetName().c_str());
	if (ImGui::BeginPopup(go->GetName().c_str()))
	{
		if (ImGui::Button("Delete"))
		{
			optionsPopup = false;

			if (go->GetParent() == app->scene->GetRoot())
				app->scene->DeleteGameObject(go);
			else
			{
				ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(ComponentType::MESH);
				if (mesh) app->renderer3D->EraseGameObject(go);

				GameObject* parent = go->GetParent();
				parent->EraseChild(go);
				if (go != nullptr) go = nullptr;
				
				go = parent;
				currentGO = parent;
				goClicked = nullptr;
			}
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Empty Child"))
		{
			optionsPopup = false;
			LOG_CONSOLE("Empty GameObject created insinde %s", go->GetName().c_str());

			static int count = 0;
			std::string name = "Empty Game Object";
			if (count > 0)
				name += " " + std::to_string(count);
			count++;

			GameObject* child = new GameObject(name);
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
