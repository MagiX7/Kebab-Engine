#pragma once

#include <map>

class Resource;
enum class ResourceType;

class ResourceManager
{
public:

	static ResourceManager* GetInstance();

	int Find(const char* fileInAssets) const;
	int ImportFile(const char* newFileInAssets);

	const Resource* GetResource(int uuid) const;
	void DeleteResource(int uuid);

private:
	ResourceManager();
	virtual ~ResourceManager();

	Resource* CreateNewResource(const char* assetsFile, ResourceType type);

private:
	static ResourceManager* instance;

	std::map<int, Resource*> resources;
};