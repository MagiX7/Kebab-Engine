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

#include "mmgr/mmgr.h"

#include <vector>
#include <queue>

//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
//#include <experimental/filesystem>
//#include <filesystem>

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

	/*for (std::vector<Texture*>::const_iterator it = textures.begin(); it != textures.end(); it++)
	{
		delete((*it));
	}*/

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
				//std::string showName = (*it).substr(0, (*it).find_last_of("_"));
				//showName = showName.substr(0, showName.find_last_of("_"));
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

			int status;

			status = remove(path.c_str());

			std::string fileName = popUpItem.substr(0, popUpItem.find_last_of("_"));
			fileName = fileName.substr(0, fileName.find_last_of("_"));
			std::string ext = popUpItem.substr(popUpItem.find_last_of("."), popUpItem.length());
			
			if (ext == ".kbmodel")
			{
				// FBX
				path = app->fileSystem->FindFilePath(fileName + ".fbx");
				if (path != "")
					status += remove(path.c_str());

				fileName = fileName + ".fbx";
				path = app->fileSystem->FindFilePath(fileName + ".meta");
				if (path != "")
					status += remove(path.c_str());

				// OBJ
				path = "";
				path = app->fileSystem->FindFilePath(fileName + ".obj");
				if (path != "")
					status += remove(path.c_str());

				fileName = fileName + ".obj";
				path = app->fileSystem->FindFilePath(fileName + ".meta");
				if (path != "")
					status += remove(path.c_str());
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

				// PNG
				path = app->fileSystem->FindFilePath(fileName + ".png");
				if (path != "")
					status += remove(path.c_str());

				fileName = fileName + ".png";
				path = app->fileSystem->FindFilePath(fileName + ".meta");
				if (path != "")
					status += remove(path.c_str());

				// DDS
				path = "";
				path = app->fileSystem->FindFilePath(fileName + ".dds");
				if (path != "")
					status += remove(path.c_str());

				fileName = fileName + ".dds";
				path = app->fileSystem->FindFilePath(fileName + ".meta");
				if (path != "")
					status += remove(path.c_str());

				// JPG
				path = "";
				path = app->fileSystem->FindFilePath(fileName + ".jpg");
				if (path != "")
					status += remove(path.c_str());

				fileName = fileName + ".jpg";
				path = app->fileSystem->FindFilePath(fileName + ".meta");
				if (path != "")
					status += remove(path.c_str());

				// TGA
				path = "";
				path = app->fileSystem->FindFilePath(fileName + ".tga");
				if (path != "")
					status += remove(path.c_str());

				fileName = fileName + ".tga";
				path = app->fileSystem->FindFilePath(fileName + ".meta");
				if (path != "")
					status += remove(path.c_str());
			}

			if (status == 0) { LOG_CONSOLE("%s Deleted Successfully!", popUpItem.c_str()); }
			else if (status < 3) { LOG_CONSOLE("Error to Delete all archives of %s", popUpItem.c_str()); }
			else { LOG_CONSOLE("Error to Delete %s", popUpItem.c_str()); }

			ImGui::CloseCurrentPopup();
			popUpItem = "";
		}
		//if (ImGui::Button("Import to Scene"))
		//{
		//	std::string path = currentFolder + popUpItem;

		//	std::string extension = popUpItem.substr(popUpItem.find_last_of("."), popUpItem.length());
		//	
		//	if (extension == ".fbx" || extension == ".obj")
		//	{
		//		app->renderer3D->Submit(MeshLoader::GetInstance()->LoadModel(path, true));
		//		LOG_CONSOLE("Asset Imported Succesfully");
		//	}
		//	else if (extension == ".dds" || extension == ".png" || extension == ".jpg")
		//	{
		//		GameObject* target = app->editor->hierarchyPanel->currentGO;
		//		if (target)
		//		{
		//			if (target->GetComponent(ComponentType::MESH))
		//			{
		//				for (int i = 0; i < target->GetComponents().size(); ++i)
		//				{
		//					ComponentMesh* mesh = (ComponentMesh*)target->GetComponent(ComponentType::MESH);
		//					GameObject* parent = target->GetParent();
		//					/*while (parent && target != parent)
		//						target = target->GetParent();*/

		//						//std::string a = (target->GetName() + '/' + name + '.' + extension);
		//					ComponentMaterial* mat = (ComponentMaterial*)target->GetComponent(ComponentType::MATERIAL);
		//					std::shared_ptr<Resource> tex = ResourceManager::GetInstance()->IsAlreadyLoaded(path);
		//					mat->AddTexture(std::static_pointer_cast<Texture>(tex));
		//				}
		//			}
		//			else
		//			{
		//				std::string message = "Couldn't apply texture, selected game object "
		//					+ target->GetName()
		//					+ " doesn't have a mesh. Try with a child game object that has a mesh instead.";
		//				LOG_CONSOLE(message.c_str());
		//			}
		//		}
		//		else LOG_CONSOLE("Please select a Game Object with mesh to apply the texture %s", popUpItem);
		//	}
		//	else
		//		LOG_CONSOLE("Asset can't be imported");

		//	ImGui::CloseCurrentPopup();
		//	popUpItem = "";
		//}

		ImGui::EndPopup();
	}
}
