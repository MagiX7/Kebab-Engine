#include "Application.h"
#include "PanelAssets.h"

#include "Input.h"
#include "Renderer3D.h"
#include "Editor.h"
#include "MainScene.h"
#include "FileSystem.h"

#include "GameObject.h"

#include "PanelHierarchy.h"

#include "mmgr/mmgr.h"

#include <vector>

AssetsPanel::AssetsPanel()
{
    active = true;
    scroll = 0;

	popUpMenu = false;
}

AssetsPanel::~AssetsPanel()
{
}

void AssetsPanel::OnRender(float dt)
{
    if(ImGui::Begin("Assets", &active))
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

		if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (int i = 0; i < assets.size(); ++i)
			{
				DisplayAsset(assets[i]);
			}
		}
    }
	ImGui::End();
}

void AssetsPanel::AddAsset(GameObject* gameObj)
{
	Asset* aux = new Asset();
	aux->gameObj = gameObj;
	aux->name = gameObj->GetName().c_str();
	aux->type = AssetType::MODEL;

	assets.push_back(aux);
}

void AssetsPanel::DisplayAsset(Asset* asset)
{
	ImGuiTreeNodeFlags flags = 0;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	flags |= ImGuiTreeNodeFlags_Leaf;

	ImGui::TreeNodeEx(asset->name.c_str(), flags);

	if (ImGui::IsItemClicked(1))
	{
		popUpMenu = true;
	}
	if (popUpMenu)
	{
		DisplayPopMenu(asset);
	}

	ImGui::TreePop();
}

void AssetsPanel::DisplayPopMenu(Asset* asset)
{
	ImGui::OpenPopup(asset->name.c_str());
	if (ImGui::BeginPopup(asset->name.c_str()))
	{
		if (ImGui::Button("Delete"))
		{
			popUpMenu = false;

			int status;
			char name[32] = "";
			sprintf_s(name, 32, "%s.fbx", asset->name.c_str());
			std::string path = app->fileSystem->FindFilePath(name, "Assets/Resources/");

			status = remove(path.c_str());
			if (status == 0) { LOG_CONSOLE("%s Deleted Successfully!", asset->name.c_str()); }
			else { LOG_CONSOLE("Error to Delete %s", asset->name.c_str()); }

			std::vector<Asset*>::iterator it;

			for (it = assets.begin(); it != assets.end(); ++it)
			{
				if (*it == asset)
				{
					assets.erase(it);
					assets.shrink_to_fit();

					app->scene->DeleteGameObject(asset->gameObj);
					break;
				}
			}
		}

		ImGui::EndPopup();
	}
}
