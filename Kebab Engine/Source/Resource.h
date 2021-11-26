#pragma once

#include <string>

enum class ResourceType
{
	NONE = 0,
	MESH,
	TEXTURE,
	MATERIAL
};

class Resource
{
public:
	Resource(/*int uuid, */ResourceType type) : /*uuid(uuid), */resourceType(type) {}
	Resource(int uuid, ResourceType type) : uuid(uuid), resourceType(type) {}
	Resource(const std::string& assets, const std::string& library) : assetsFile(assets), libraryFile(library) {}
	virtual ~Resource() {};

	inline const ResourceType& GetResourceType() { return resourceType; }
	inline const int& GetUUID() { return uuid; }
	inline void SetUUID(int newUuid) { uuid = newUuid; }
	inline void SetAssetsPath(const std::string& p) { assetsFile = p; }
	inline void SetLibraryPath(const std::string& l) { libraryFile= l; }

public:
	int uuid;

protected:
	ResourceType resourceType;

	std::string assetsFile;
	std::string libraryFile;

};