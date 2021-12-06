#pragma once

#include "Vertex.h"

#include "Texture.h"
//#include "TextureProperties.h"

#include <vector>
#include <string>
#include <map>
#include <memory>

class Resource;
class KbModel;
class KbMesh;
enum class ResourceType;

class ResourceManager
{
public:

	static ResourceManager* GetInstance();

	void CleanUp();


	std::shared_ptr<Resource> GetResource(int uuid) const;
	void DeleteResource(int uuid);

	void AddResource(KbMesh* res);
	void AddResource(Texture* res);

	bool IsAlreadyLoaded(int uuid);
	std::shared_ptr<Resource> IsAlreadyLoaded(const char* assetsFile);
	std::shared_ptr<Resource> IsAlreadyLoaded(const std::string& libraryFile);

	int GetReferenceCount(int uuid);

	std::shared_ptr<KbModel> CreateModel(const char* assetsFile);
	std::shared_ptr<Resource> CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& name);
	std::shared_ptr<Texture> CreateTexture(const char* assetsFile, int modelUuid = 0, const TextureProperties& props = TextureProperties(), int texUuid = -1);

	std::shared_ptr<Resource> LoadTexture(const char* libraryFile, int uuid);

	int GenerateUUID();

	std::shared_ptr<Resource> FindMetaData(const char* assetsFile);

	std::shared_ptr<KbModel> LoadModelMetaData(const char* assetsFile);
	std::shared_ptr<Texture> LoadTextureMetaData(const char* assetsFile);
private:
		
	ResourceManager();
	virtual ~ResourceManager();

private:
	static ResourceManager* instance;

	std::map<int, std::shared_ptr<Resource>> resources;

	TextureProperties defaultTextureProps;

};