#include "Application.h"
#include "FileSystem.h"


#include "ResourceManager.h"
#include "Resource.h"


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

	Resource* res = CreateNewResource(newFileInAssets, type);
	
	char* fileBuffer;
	app->fileSystem->Load(newFileInAssets, &fileBuffer);




	return 0;
}

const Resource* ResourceManager::GetResource(int uuid) const
{
	return nullptr;
}

void ResourceManager::DeleteResource(int uuid)
{
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{

}

Resource* ResourceManager::CreateNewResource(const char* assetsFile, ResourceType type)
{
	return nullptr;
}
