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
	Resource(int uuid, ResourceType type) : uuid(uuid), resourceType(type) {}
	virtual ~Resource() {};

	inline const ResourceType& GetResourceType() { return resourceType; }
	inline const int& GetUUID() { return uuid; }


protected:
	ResourceType resourceType;
	int uuid;

	std::string assetsFile;
	std::string libraryFile;

};