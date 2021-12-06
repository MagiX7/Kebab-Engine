#include "Application.h"
#include "PanelAssets.h"

#include "Input.h"
#include "Renderer3D.h"
#include "Editor.h"
#include "MainScene.h"
#include "FileSystem.h"

#include "ResourceManager.h"
#include "Model.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "PanelHierarchy.h"
#include "PanelImportModel.h"
#include "PanelImportTexture.h"

#include "TextureLoader.h"
#include "MeshLoader.h"

#include <vector>
#include <queue>

#include "mmgr/mmgr.h"


AssetsPanel::AssetsPanel()
{
    active = true;
    scroll = 0;

	columns = 10;
	popUpItem = "";
	popUpMenu = false;
	rename = false;

	timer = 0;

	entryFolder = "Library/";
	currentFolder = entryFolder;

	folderTex = TextureLoader::GetInstance()->LoadTextureCustomFormat("Library/Textures/folder_icon.kbtexture");
	modelTex = TextureLoader::GetInstance()->LoadTextureCustomFormat("Library/Textures/model_icon.kbtexture");
	pngTex = TextureLoader::GetInstance()->LoadTextureCustomFormat("Library/Textures/texture_icon.kbtexture");

	if (folderTex == nullptr)
		folderTex = TextureLoader::GetInstance()->LoadTexture("Assets/Resources/Icons/folder_icon.png");
	if (modelTex == nullptr)
		modelTex = TextureLoader::GetInstance()->LoadTexture("Assets/Resources/Icons/model_icon.png");
	if (pngTex == nullptr)
		pngTex = TextureLoader::GetInstance()->LoadTexture("Assets/Resources/Icons/texture_icon.png");
}

AssetsPanel::~AssetsPanel()
{
	delete folderTex;
	delete modelTex;
	delete pngTex;

	textures.clear();
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

			if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				popUpMenu = true;
		}

		RefreshAssets(dt);

		DisplayAssets();

		if (popUpItem != "")
			DisplayItemPopMenu();

		if (popUpMenu)
			DisplayPopMenu();
	}
	ImGui::End();
}

void AssetsPanel::LoadAssetsToCustom()
{	
	std::queue<std::string> que;

	std::string entry = "Assets/";
	que.push(entry);

	while (!que.empty())
	{
		std::string currentFolderToLoad = que.front();
		que.pop();

		std::vector<std::string> dirList;
		std::vector<std::string> fileList;

		app->fileSystem->DiscoverFiles(currentFolderToLoad.c_str(), fileList, dirList);

		for (std::vector<std::string>::const_iterator it = dirList.begin(); it != dirList.end(); it++)
		{
			std::string folderPath = currentFolderToLoad + (*it) + "/";
			que.push(folderPath);
		}

		for (std::vector<std::string>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
		{
			std::string completePath = currentFolderToLoad + (*it);
			std::string ext = (*it).substr((*it).find_last_of("."), (*it).length());

			if (ext == ".fbx" || ext == ".obj")
			{
				// Resource managed inside this function
				MeshLoader::GetInstance()->LoadModel(completePath);
			}
			else if (ext == ".png" || ext == ".dds" || ext == ".jpg" || ext == ".tga")
			{
				if (currentFolderToLoad != "Assets/Resources/Icons/")
				{
					std::shared_ptr<Texture> tex = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->IsAlreadyLoaded(completePath.c_str()));
					if (!tex)
					{
						tex = ResourceManager::GetInstance()->CreateTexture(completePath.c_str());
						tex->SetAssetsPath(completePath);
						TextureLoader::GetInstance()->SaveTextureCustomFormat(tex.get(), 0);
					}
					textures.push_back(tex.get());
				}
				else
				{
					ResourceManager::GetInstance()->CreateTexture(completePath.c_str());
				}
			}
		}
	}
}

void AssetsPanel::DisplayAssets()
{
	std::vector<std::string> dirList;
	std::vector<std::string> fileList;

	app->fileSystem->DiscoverFiles(currentFolder.c_str(), fileList, dirList);

	if ((ImGui::GetWindowWidth() / 140) < columns - 1)
		columns--;
	else if ((ImGui::GetWindowWidth() / 140) > columns + 1)
		columns++;

	ImGui::Columns(columns, 0, false);

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
		std::string dragpath = currentFolder + (*it);
		ImGui::PushID(dragpath.c_str());

		std::string nameToShow = (*it).substr(0, (*it).find_last_of("_"));
		nameToShow = nameToShow.substr(0, nameToShow.find_last_of("_"));

		std::string aux = (*it).substr((*it).find_last_of("."), (*it).length());
		if (aux == ".fbx" || aux == ".obj" || aux == ".kbmodel")
			ImGui::ImageButton((ImTextureID)modelTex->GetID(), { 100,100 });
		else if (aux == ".dds" || aux == ".png" || aux == ".jpg" || aux == ".kbtexture")
		{

			ImTextureID id = (ImTextureID)pngTex->GetID();

			for (std::vector<Texture*>::const_iterator itTex = textures.begin(); itTex != textures.end(); itTex++)
			{
				std::string n = (*it).substr(0, (*it).find_last_of("."));
				nameToShow = n;
				if ((*itTex)->GetName() == n)
				{
					id = (ImTextureID)(*itTex)->GetID();
					break;
				}
			}

			ImGui::ImageButton(id, { 100,100 });
		}
		else
			ImGui::Button((*it).c_str(), { 100,100 });

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ASSET_ITEM", dragpath.c_str(), dragpath.size() + 1);
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			popUpItem = (*it).c_str();

		else if (ImGui::IsItemClicked(ImGuiMouseButton_Middle))
		{
			if (aux == ".kbmodel")
			{
				std::string assPath = app->fileSystem->FindFilePath(nameToShow + ".fbx");
				if (assPath.empty())
					assPath = app->fileSystem->FindFilePath(nameToShow + ".obj");

				app->editor->panelImportModel->SetAssetsPath(assPath);
				app->editor->panelImportModel->active = !app->editor->panelImportModel->active;
			}

			else if (aux == ".kbtexture")
			{
				std::string assPath = app->fileSystem->FindFilePath(nameToShow + ".png");
				if (assPath.empty())
					assPath = app->fileSystem->FindFilePath(nameToShow + ".jpg");
				if (assPath.empty())
					assPath = app->fileSystem->FindFilePath(nameToShow + ".dds");

				app->editor->panelImportTexture->SetTexturePath(assPath.c_str());
				app->editor->panelImportTexture->active = !app->editor->panelImportTexture->active;
			}
		}


		ImGui::Text(nameToShow.c_str());

		ImGui::NextColumn();


		ImGui::PopID();
	}
}


void AssetsPanel::RefreshAssets(float dt)
{
	timer += dt;

	if (timer >= 10)
	{
		timer = 0;

		std::queue<std::string> que;

		std::string entry = "Assets/";
		que.push(entry);

		while (!que.empty())
		{
			std::string currentFolderToLoad = que.front();
			que.pop();

			std::vector<std::string> dirList;
			std::vector<std::string> fileList;

			app->fileSystem->DiscoverFiles(currentFolderToLoad.c_str(), fileList, dirList);

			for (std::vector<std::string>::const_iterator it = dirList.begin(); it != dirList.end(); it++)
			{
				std::string folderPath = currentFolderToLoad + (*it) + "/";
				que.push(folderPath);
			}

			for (std::vector<std::string>::const_iterator it = fileList.begin(); it != fileList.end(); it++)
			{
				std::string completePath = currentFolderToLoad + (*it);
				std::string ext = (*it).substr((*it).find_last_of("."), (*it).length());

				if (ext == ".fbx" || ext == ".obj")
				{
					if (!ResourceManager::GetInstance()->IsAlreadyLoaded(completePath.c_str()))
					{
						// Resource managed inside this function
						MeshLoader::GetInstance()->LoadModel(completePath);
					}
				}
				else if (ext == ".png" || ext == ".dds" || ext == ".jpg" || ext == ".tga")
				{
					if (!ResourceManager::GetInstance()->IsAlreadyLoaded(completePath.c_str()))
					{
						if (currentFolderToLoad != "Assets/Resources/Icons/")
						{
							std::shared_ptr<Texture> tex = ResourceManager::GetInstance()->CreateTexture(completePath.c_str());
							tex->SetAssetsPath(completePath);
							TextureLoader::GetInstance()->SaveTextureCustomFormat(tex.get(), 0);
							textures.push_back(tex.get());
						}
						else
						{
							ResourceManager::GetInstance()->CreateTexture(completePath.c_str());
						}
					}
				}
			}
		}
	}
}

void AssetsPanel::DisplayPopMenu()
{
	ImGui::OpenPopup("PopMenu");

	if (!ImGui::IsAnyItemHovered() && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		rename = false;
		popUpMenu = false;
	}

	if (ImGui::BeginPopup("PopMenu"))
	{
		if (ImGui::Button("Create Folder"))
		{
			ImGui::OpenPopup("Folder Name");
			rename = true;
		}

		if (rename)
		{
			ImGui::BeginPopup("Folder Name");

			static char name[32] = "Default";
			ImGui::Text("Edit name:");
			if (ImGui::InputText("##edit", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::string folderName = name;
				folderName = currentFolder + "/" + name;
				app->fileSystem->CreateDirectoryA(folderName.c_str());

				popUpMenu = false;
				rename = false;
			}

			ImGui::EndPopup();
		}

		ImGui::EndPopup();
	}
}

void AssetsPanel::DisplayItemPopMenu()
{
	ImGui::OpenPopup(popUpItem.c_str());

	if (!ImGui::IsAnyItemHovered() && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		popUpItem = "";

	if (ImGui::BeginPopup(popUpItem.c_str()))
	{
		if (ImGui::Button("Delete"))
		{
			std::string path = currentFolder + popUpItem;

			int status = 0;

			std::string fileName = popUpItem.substr(0, popUpItem.find_last_of("_"));
			fileName = fileName.substr(0, fileName.find_last_of("_"));
			std::string ext = popUpItem.substr(popUpItem.find_last_of("."), popUpItem.length());
			
			if (ext == ".kbmodel")
			{
				std::shared_ptr<KbModel> mod = std::static_pointer_cast<KbModel>(ResourceManager::GetInstance()->IsAlreadyLoaded(path));
				std::vector<KbMesh*> meshes = mod->GetMeshes();

				status += remove(mod->GetLibraryPath().c_str());
				status += remove(mod->GetAssetsPath().c_str());
				status += remove(mod->GetMetaPath().c_str());

				for (std::vector<KbMesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
				{
					status += remove((*it)->GetLibraryPath().c_str());
					status += remove((*it)->GetAssetsPath().c_str());
					status += remove((*it)->GetMetaPath().c_str());
				}

				meshes.clear();

				if (status == 0) { LOG_CONSOLE("%s Deleted Successfully!", popUpItem.c_str()); }
				else { LOG_CONSOLE("Error to Delete %s", popUpItem.c_str()); }
			}
			else if (ext == ".kbtexture")
			{
				// Delete the texture
				std::vector<Texture*>::iterator it = textures.begin();
				for (; it != textures.end(); ++it)
				{
					if ((*it)->GetPath() == path)
					{
						textures.erase(it);
						break;
					}
				}

				std::shared_ptr<Texture> tex = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->IsAlreadyLoaded(path));
				status += remove(tex->GetLibraryPath().c_str());
				status += remove(tex->GetAssetsPath().c_str());
				status += remove((tex->GetAssetsPath() + ".meta").c_str());

				if (status == 0) { LOG_CONSOLE("%s Deleted Successfully!", popUpItem.c_str()); }
				else { LOG_CONSOLE("Error to Delete %s", popUpItem.c_str()); }
			}
			else
			{
				LOG_CONSOLE("Error to Delete %s", popUpItem.c_str());
			}

			ImGui::CloseCurrentPopup();
			popUpItem = "";
		}

		ImGui::EndPopup();
	}
}
