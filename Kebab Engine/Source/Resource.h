#pragma once

#include <string>

enum class ResourceType
{
	NONE = 0,
	MESH,
	TEXTURE,
	MODEL,
	SHADER,
};

class Resource
{
public:
	Resource(ResourceType type) : resourceType(type) {}
	Resource(int uuid, ResourceType type) : uuid(uuid), resourceType(type) {}
	Resource(const std::string& assets, const std::string& library) : assetsFile(assets), libraryFile(library) {}
	virtual ~Resource()
	{
		assetsFile.clear();
		libraryFile.clear();
		metaFile.clear();
	};

	inline const ResourceType& GetResourceType() { return resourceType; }
	inline const int& GetUUID() { return uuid; }
	inline void SetUUID(int newUuid) { uuid = newUuid; }
	inline void SetAssetsPath(const std::string& p) { assetsFile = p; }
	inline void SetLibraryPath(const std::string& l) { libraryFile= l; }
	inline void SetMetaPath(const std::string& m) { metaFile = m; }

	inline const std::string& GetAssetsPath() { return assetsFile; }
	inline const std::string& GetLibraryPath() { return libraryFile; }
	inline const std::string& GetMetaPath() { return metaFile; }

	inline const bool HasMetaFile() const { return hasMetaFile; }

	virtual void CreateMetaDataFile(const char* assetsFile) {};
	virtual void LoadMetaDataFile() {};


public:
	int uuid;

protected:
	ResourceType resourceType;

	std::string assetsFile;
	std::string libraryFile;
	std::string metaFile;

	bool hasMetaFile = false;

};