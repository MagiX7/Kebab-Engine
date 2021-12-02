#include "Application.h"
#include "PanelInspector.h"

#include "Input.h"
#include "Editor.h"
#include "MainScene.h"

#include "PanelHierarchy.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "QdTree.h"

#include <vector>
#include <queue>

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
			ImGui::TextColored({ 0.1f, 0.9f, 0.9f, 1.0f }, app->editor->hierarchyPanel->currentGO->GetName().c_str());

			if (ImGui::Checkbox("Static", &app->editor->hierarchyPanel->currentGO->isStatic))
			{
				if (app->editor->hierarchyPanel->currentGO->isStatic)
				{
					std::queue<GameObject*> que;
					que.push(app->editor->hierarchyPanel->currentGO);

					while (!que.empty())
					{
						GameObject* go = que.front();
						que.pop();

						go->isStatic = app->editor->hierarchyPanel->currentGO->isStatic;
						app->scene->rootQT->Insert(go);
						app->scene->rootQT->Recalculate();

						for (std::vector<GameObject*>::iterator it = go->GetChilds().begin(); it != go->GetChilds().end(); ++it)
						{
							que.push((*it));
						}
					}
				}
				else
				{
					std::queue<GameObject*> que;
					que.push(app->editor->hierarchyPanel->currentGO);

					while (!que.empty())
					{
						GameObject* go = que.front();
						que.pop();

						go->isStatic = app->editor->hierarchyPanel->currentGO->isStatic;
						app->scene->rootQT->Remove(go);
						app->scene->rootQT->Recalculate();

						for (std::vector<GameObject*>::iterator it = go->GetChilds().begin(); it != go->GetChilds().end(); ++it)
						{
							que.push((*it));
						}
					}
				}
			}

			ImGui::Separator();

			std::vector<Component*>::const_iterator it = app->editor->hierarchyPanel->currentGO->GetComponents().begin();

			for (; it != app->editor->hierarchyPanel->currentGO->GetComponents().end(); ++it)
			{
				(*it)->DrawOnInspector();
			}

			ImGui::Separator();

			if (ImGui::Button("Add Component")) ImGui::OpenPopup("Add Component");;

			if (ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::Button("Mesh"))
				{
					if (app->editor->hierarchyPanel->currentGO->GetComponent(ComponentType::MESH) == nullptr)
					{
						ComponentMesh* newComp = new ComponentMesh(app->editor->hierarchyPanel->currentGO);

						app->editor->hierarchyPanel->currentGO->AddComponent(newComp);
					}
					else LOG_CONSOLE("Already Exist a this Component on %s", app->editor->hierarchyPanel->currentGO->GetName().c_str());
					
					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::Button("Material"))
				{
					if (app->editor->hierarchyPanel->currentGO->GetComponent(ComponentType::MATERIAL) == nullptr)
					{
						ComponentMaterial* newComp = new ComponentMaterial(app->editor->hierarchyPanel->currentGO);

						app->editor->hierarchyPanel->currentGO->AddComponent(newComp);
					}
					else LOG_CONSOLE("Already Exist a this Component on %s", app->editor->hierarchyPanel->currentGO->GetName().c_str());
					
					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::Button("Transform"))
				{
					if (app->editor->hierarchyPanel->currentGO->GetComponent(ComponentType::TRANSFORM) == nullptr)
					{
						ComponentTransform* newComp = new ComponentTransform(app->editor->hierarchyPanel->currentGO);

						app->editor->hierarchyPanel->currentGO->AddComponent(newComp);
					}
					else LOG_CONSOLE("Already Exist a this Component on %s", app->editor->hierarchyPanel->currentGO->GetName().c_str());
					
					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::Button("Camera"))
				{
					if (app->editor->hierarchyPanel->currentGO->GetComponent(ComponentType::CAMERA) == nullptr)
					{
						ComponentCamera* newComp = new ComponentCamera(app->editor->hierarchyPanel->currentGO, CameraType::GAME);

						app->editor->hierarchyPanel->currentGO->AddComponent(newComp);
					}
					else LOG_CONSOLE("Already Exist a this Component on %s", app->editor->hierarchyPanel->currentGO->GetName().c_str());
					
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}
}