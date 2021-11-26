#pragma once

#include <map>
#include <memory>

class Resource;
enum class ResourceType;

class ResourceManager
{
public:

	static ResourceManager* GetInstance();

	int Find(const char* fileInAssets) const;
	int ImportFile(const char* newFileInAssets);

	std::shared_ptr<Resource> GetResource(int uuid) const;
	void DeleteResource(int uuid);

	void AddResource(Resource* res);

	bool IsAlreadyLoaded(int uuid);

	int GetReferenceCount(int uuid);

	std::shared_ptr<Resource> CreateNewResource(const char* assetsFile, ResourceType type);
private:
	ResourceManager();
	virtual ~ResourceManager();


	int GenerateUUID();

private:
	static ResourceManager* instance;

	std::map<int, std::shared_ptr<Resource>> resources;


	int uuid = -2;
};