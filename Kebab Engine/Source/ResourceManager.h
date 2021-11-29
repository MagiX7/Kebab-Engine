#pragma once

#include "Vertex.h"

#include "Texture.h"
//#include "TextureProperties.h"

#include <vector>
#include <string>
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
	std::shared_ptr<Resource> IsAlreadyLoaded(const char* assetsFile);

	int GetReferenceCount(int uuid);

	std::shared_ptr<Resource> CreateNewResource(const char* assetsFile, ResourceType type, int uuid = 0);
	std::shared_ptr<Resource> CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& name);
	std::shared_ptr<Resource> CreateTexture(const char* assetsFile, int modelUuid, const TextureProperties& props = TextureProperties());

	int GenerateUUID();

	std::shared_ptr<Resource> FindMetaData(const char* assetsFile);

private:
	ResourceManager();
	virtual ~ResourceManager();

private:
	static ResourceManager* instance;

	std::map<int, std::shared_ptr<Resource>> resources;

	TextureProperties defaultTextureProps;

};