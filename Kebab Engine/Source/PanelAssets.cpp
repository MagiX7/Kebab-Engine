#include "Application.h"
#include "PanelAssets.h"

#include "Input.h"
#include "Renderer3D.h"
#include "Editor.h"
#include "MainScene.h"
#include "FileSystem.h"

#include "GameObject.h"

#include "PanelHierarchy.h"

#include "TextureLoader.h"

#include "mmgr/mmgr.h"

#include <vector>

//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
//#include <experimental/filesystem>
//#include <filesystem>

AssetsPanel::AssetsPanel()
{
    active = true;
    scroll = 0;

	popUpMenu = false;

	entryFolder = "Assets/";
	currentFolder = entryFolder;

	folderTex = TextureLoader::GetInstance()->LoadTexture("Assets/Resources/folder_icon.png");
	modelTex = TextureLoader::GetInstance()->LoadTexture("Assets/Resources/model_icon.png");
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

		DisplayAssets();

    }
	ImGui::End();
}

void AssetsPanel::AddAsset(GameObject* gameObj)
{
	Asset* aux = new Asset();
	aux->gameObj = gameObj;
	aux->name = gameObj->GetName().c_str();
	aux->type = AssetType::FBX;

	assets.push_back(aux);
}

void AssetsPanel::DisplayAssets()
{
	std::vector<std::string> dirList;
	std::vector<std::string> fileList;

	app->fileSystem->DiscoverFiles(currentFolder.c_str(), fileList, dirList);

	ImGui::Columns(10, 0, false);

	if (currentFolder != entryFolder)
	{
		if (ImGui::Button("Back", { 100,100 }));
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			std::string prev = currentFolder.substr(0, currentFolder.find_last_of("/"));
			prev = prev.substr(0, prev.find_last_of("/") + 1);
			currentFolder = prev;
		}

		ImGui::NextColumn();
	}

	for (std::vector<std::string>::const_iterator it = dirList.begin(); it != dirList.end(); it++)
	{
		ImGui::ImageButton((ImTextureID)folderTex->GetID(), { 100,100 });
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			char aux[128] = "";
				sprintf_s(aux, 128, "%s%s", currentFolder.c_str(), (*it).c_str());

				if (app->fileSystem->IsDirectory(aux))
				{
					sprintf_s(aux, 128, "%s%s/", currentFolder.c_str(), (*it).c_str());
						currentFolder = aux;
				}
		}

		ImGui::Text((*it).c_str());

		ImGui::NextColumn();
	}

	for (std::vector<std::string>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
	{
		std::string aux = (*it).substr((*it).find_last_of("."), (*it).length());
		if (strcmp(aux.c_str(), ".fbx") == 0)
			ImGui::ImageButton((ImTextureID)modelTex->GetID(), { 100,100 });
		else
			ImGui::Button((*it).c_str(), { 100,100 });
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			
		}

		ImGui::Text((*it).substr(0, (*it).find_last_of(".")).c_str());

		ImGui::NextColumn();
	}

}

void AssetsPanel::DisplayPopMenu(Asset* asset)
{
	ImGui::OpenPopup(asset->name.c_str());
	if (ImGui::BeginPopup(asset->name.c_str()))
	{
		if (ImGui::Button("Delete"))
		{
			popUpMenu = false;

			char name[32] = "";
			sprintf_s(name, 32, "%s.fbx", asset->name.c_str());

			char path[128] = "";
			app->fileSystem->FindFilePath(name, path, currentFolder.c_str());

			char auxPath[128] = "";
			sprintf_s(auxPath, 128, "%s", path);

			int status;
			status = remove(auxPath);
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
		if (ImGui::Button("Import to Scene"))
		{
			//app->renderer3D->Submit(MeshLoader::GetInstance()->LoadModel("Assets/Resources/Baker House.fbx"));
		}

		ImGui::EndPopup();
	}
}
