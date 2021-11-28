#include "Application.h"
#include "FileSystem.h"

#include "ResourceManager.h"
#include "Resource.h"

#include "MeshLoader.h"
#include "TextureLoader.h"

#include "Model.h"

#include <Algorithm/Random/LCG.cpp>


ResourceManager* ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::GetInstance()
{
	if (!instance) instance = new ResourceManager();
	return instance;
}

int ResourceManager::Find(const char* fileInAssets) const
{
	return 0;
}

int ResourceManager::ImportFile(const char* newFileInAssets)
{
	int ret = 0;
	ResourceType type = ResourceType::NONE;

	std::string s = newFileInAssets;

	int start = s.find_last_of('.');
	if (s.substr(start) == ".fbx" || s.substr(start) == ".obj")
	{
		type = ResourceType::MESH;
	}
	else if (s.substr(start) == ".png" || s.substr(start) == ".jpg")
	{
		type = ResourceType::TEXTURE;
	}

	std::shared_ptr<Resource> res = CreateNewResource(newFileInAssets, type);
	
	char* fileBuffer;
	app->fileSystem->Load(newFileInAssets, &fileBuffer);


	return 0;
}

std::shared_ptr<Resource> ResourceManager::GetResource(int uuid) const
{
	std::map<int, std::shared_ptr<Resource>>::const_iterator it = resources.find(uuid);
	std::map<int, std::shared_ptr<Resource>>::const_iterator itEnd = resources.end();

	if (it != itEnd)
	{
		return (*it).second;
	}

	return nullptr;
}

void ResourceManager::DeleteResource(int uuid)
{
	//std::map<int, std::shared_ptr<Resource*>>::iterator it = resources.find(uuid);
	//if (it != resources.end() && std::shared_ptr<Resource*>::unique)
	//{

	//}
}

void ResourceManager::AddResource(Resource* res)
{
	//if (!IsAlreadyLoaded(res->GetUUID()))
	//{
		std::pair<int, std::shared_ptr<Resource>> p;
		p.second = std::make_shared<Resource>(res->GetResourceType());
		uuid = GenerateUUID();
		res->uuid = uuid;
		//res->SetUUID(uuid);
		p.first = res->GetUUID();

		resources.insert(resources.end(), p);
	//}
}

bool ResourceManager::IsAlreadyLoaded(int uuid)
{
	std::map<int, std::shared_ptr<Resource>>::iterator it = resources.find(uuid);
	if (it == resources.end())
		return false;

	return true;
}

std::shared_ptr<Resource> ResourceManager::IsAlreadyLoaded(const char* assetsFile)
{
	std::map<int, std::shared_ptr<Resource>>::iterator it = resources.begin();

	for (; it != resources.end(); ++it)
	{
		/*if ((*it).second.get()->HasMetaFile())
		{
			return FindMetaData(assetsFile);
		}*/

		if ((*it).second.get()->GetAssetsPath() == assetsFile)
		{
			return (*it).second;
		}
	}

	//return FindMetaData(assetsFile);

	return nullptr;
}

int ResourceManager::GetReferenceCount(int uuid)
{
	std::map<int, std::shared_ptr<Resource>>::iterator it = resources.find(uuid);

	if (it != resources.end())
	{
		return it->second.use_count();
	}
	return -1;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

std::shared_ptr<Resource> ResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{
	std::shared_ptr<Resource> ret = nullptr;

	switch(type)
	{
		case ResourceType::TEXTURE:
		{
			if(strcmp(assetsFile, "Checkers") != 0)
			{
				Texture* tex = TextureLoader::GetInstance()->LoadTexture(assetsFile);
				tex->uuid = GenerateUUID();
				ret = (std::shared_ptr<Resource>)std::make_shared<Texture>(*tex);

				ret.get()->SetAssetsPath(assetsFile);
				std::string tmp = assetsFile;
				int start = tmp.find_last_of("/");
				int end = tmp.find(".");
				std::string lib = "Library/Textures/" + tmp.substr(start + 1, end - start - 1) + "__" + std::to_string(tex->uuid) + ".kbtexture";
				ret.get()->SetLibraryPath(lib);

				/*delete tex;
				tex = nullptr;*/
				
				resources[ret.get()->uuid] = ret;
			}

			break;
		}

		case ResourceType::MODEL:
		{
			KbModel* model = new KbModel();

			model->uuid = GenerateUUID();
			ret = (std::shared_ptr<Resource>)std::make_shared<KbModel>(*model);

			ret.get()->SetAssetsPath(assetsFile);
			std::string tmp = assetsFile;
			int start = tmp.find_last_of("/");
			int end = tmp.find(".");
			std::string lib = "Library/Models/" + tmp.substr(start + 1, end - start - 1) + "__" + std::to_string(model->uuid) + ".kbmodel";
			ret.get()->SetLibraryPath(lib);

			//ret.get()->CreateMetaDataFile(assetsFile);

			delete model;
			model = nullptr;

			resources[ret.get()->uuid] = ret;
		}
	}
	return ret;
}

std::shared_ptr<Resource> ResourceManager::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& name)
{
	std::shared_ptr<Resource> ret = nullptr;

	//KbMesh* mesh = new KbMesh(vertices, indices);
	//mesh->uuid = GenerateUUID();
	//
	//ret = (std::shared_ptr<Resource>)std::make_shared<KbMesh>(*mesh);
	//
	//MeshLoader::GetInstance()->SaveMeshCustomFormat((KbMesh*)ret.get(), name);
	//
	//resources[ret.get()->uuid] = ret;

	return ret;
}

int ResourceManager::GenerateUUID()
{
	LCG lgc = LCG();
	return lgc.IntFast();
}

std::shared_ptr<Resource> ResourceManager::FindMetaData(const char* assetsFile)
{
	std::map<int, std::shared_ptr<Resource>>::iterator it = resources.begin();

	for (; it != resources.end(); ++it)
	{
		if ((*it).second.get()->HasMetaFile())
		{
			//(*it).second.get().LoadMetaFile();
			(*it).second.get()->LoadMetaDataFile();

			return (*it).second;
		}
	}

	return nullptr;


	//std::string libExt = "noext";
	//std::string s = assetsFile;
	//int startExt = s.find(".");
	//std::string ext = s.substr(startExt);

	//std::string libPath = "Library/";
	//
	//if (ext == ".fbx" || ext == ".obj") // We do not check meshes because they belong to a model. If the model is loaded, the meshes are too
	//{
	//	libPath += "Models/";
	//	libExt = ".kbmodel";
	//}
	//
	//else if (ext == ".png" || ext == ".jpg" || ext == ".dds")
	//{
	//	libPath += "Textures/";
	//	libExt = ".kbtexture";
	//}

	//int startName = s.find_last_of("/");
	//std::string name = s.substr(startName + 1, startExt - startName - 1);

	//libPath += name/* + libExt*/;


	//// Assets/Resources/Avril.fbx
}