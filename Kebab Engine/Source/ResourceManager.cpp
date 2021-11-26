#include "Application.h"
#include "FileSystem.h"

#include "ResourceManager.h"
#include "Resource.h"

#include "MeshLoader.h"
#include "TextureLoader.h"

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
				Texture t = *tex;
				ret = (std::shared_ptr<Resource>)std::make_shared<Texture>(t);

				ret.get()->SetAssetsPath(assetsFile);
				
				resources[ret.get()->uuid] = ret;
			}

			break;
		}
		
		case ResourceType::MESH:
		{

			ret = std::make_shared<Resource>(type);


			break;
		}
	}
	return ret;
}

int ResourceManager::GenerateUUID()
{
	LCG lgc = LCG();
	return lgc.IntFast();
}
