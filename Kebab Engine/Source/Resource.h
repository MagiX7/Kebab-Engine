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
	virtual ~Resource() {};

	inline const ResourceType& GetResourceType() { return resourceType; }
	inline const int& GetUUID() { return uuid; }
	inline void SetUUID(int newUuid) { uuid = newUuid; }

public:
	int uuid;

protected:
	ResourceType resourceType;

	std::string assetsFile;
	std::string libraryFile;

};