#include "Application.h"
#include "FileSystem.h"

#include "ResourceManager.h"
#include "Resource.h"

#include "MeshLoader.h"
#include "TextureLoader.h"

#include "Model.h"

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
	KbMesh* m = (KbMesh*)res;
	std::shared_ptr<Resource> resource = std::make_shared<KbMesh>(*m);
	resources[res->uuid] = resource;
}

bool ResourceManager::IsAlreadyLoaded(int uuid)
{
	std::map<int, std::shared_ptr<Resource>>::iterator it = resources.find(uuid);
	if (it == resources.end())
		return false;

	return true;
}

std::shared_ptr<Resource> ResourceManager::IsAlreadyLoaded(const char* assetsFile)
{
	std::map<int, std::shared_ptr<Resource>>::iterator it = resources.begin();

	for (; it != resources.end(); ++it)
	{
		/*if ((*it).second.get()->HasMetaFile())
		{
			return FindMetaData(assetsFile);
		}*/

		if ((*it).second.get()->GetAssetsPath() == assetsFile)
		{
			return (*it).second;
		}
	}

	//return FindMetaData(assetsFile);

	return nullptr;
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

std::shared_ptr<Resource> ResourceManager::CreateNewResource(const char* assetsFile, ResourceType type, int uuid)
{
	std::shared_ptr<Resource> ret = nullptr;

	switch(type)
	{
		case ResourceType::TEXTURE:
		{
			//if(strcmp(assetsFile, "Checkers") != 0)
			//{
			//	Texture* tex = TextureLoader::GetInstance()->LoadTexture(assetsFile);
			//	tex->uuid = GenerateUUID();
			//	ret = (std::shared_ptr<Resource>)std::make_shared<Texture>(*tex);

			//	ret.get()->SetAssetsPath(assetsFile);
			//	std::string tmp = assetsFile;
			//	int start = tmp.find_last_of("/");
			//	int end = tmp.find(".");
			//	std::string lib = "Library/Textures/" + tmp.substr(start + 1, end - start - 1) +
			//		"__" + std::to_string(uuid) + ".kbtexture";
			//	ret.get()->SetLibraryPath(lib);

			//	/*delete tex;
			//	tex = nullptr;*/
			//	
			//	resources[ret.get()->uuid] = ret;
			//}

			break;
		}

		case ResourceType::MODEL:
		{
			KbModel* model = new KbModel();

			model->uuid = GenerateUUID();
			ret = (std::shared_ptr<Resource>)std::make_shared<KbModel>(*model);

			ret.get()->SetAssetsPath(assetsFile);
			std::string tmp = assetsFile;
			int start = tmp.find_last_of("/");
			int end = tmp.find(".");
			std::string lib = "Library/Models/" + tmp.substr(start + 1, end - start - 1) + "__" + std::to_string(model->uuid) + ".kbmodel";
			ret.get()->SetLibraryPath(lib);

			//ret.get()->CreateMetaDataFile(assetsFile);

			delete model;
			model = nullptr;

			resources[ret.get()->uuid] = ret;
		}
	}
	return ret;
}

std::shared_ptr<Resource> ResourceManager::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& name)
{
	std::shared_ptr<Resource> ret = nullptr;

	//KbMesh* mesh = new KbMesh(vertices, indices);
	//mesh->uuid = GenerateUUID();
	//
	//ret = (std::shared_ptr<Resource>)std::make_shared<KbMesh>(*mesh);
	//
	//MeshLoader::GetInstance()->SaveMeshCustomFormat((KbMesh*)ret.get(), name);
	//
	//resources[ret.get()->uuid] = ret;

	return ret;
}

std::shared_ptr<Resource> ResourceManager::CreateTexture(const char* assetsFile, int modelUuid, const TextureProperties& props)
{
	std::shared_ptr<Resource> ret = nullptr;

	Texture* tex = TextureLoader::GetInstance()->LoadTexture(assetsFile, props);
	tex->uuid = GenerateUUID();
	ret = (std::shared_ptr<Resource>)std::make_shared<Texture>(*tex);

	ret.get()->SetAssetsPath(assetsFile);
	std::string tmp = assetsFile;
	int start = tmp.find_last_of("/");
	int end = tmp.find(".");
	std::string lib = "Library/Textures/" + tmp.substr(start + 1, end - start - 1) +
		"__" + std::to_string(modelUuid == 0 ? tex->uuid : modelUuid) + ".kbtexture";
	ret.get()->SetLibraryPath(lib);

	ret.get()->CreateMetaDataFile(assetsFile);

	/*delete tex;
	tex = nullptr;*/

	resources[ret.get()->uuid] = ret;


	return ret;
}

int ResourceManager::GenerateUUID()
{
	LCG lgc = LCG();
	return lgc.IntFast();
}

std::shared_ptr<Resource> ResourceManager::FindMetaData(const char* assetsFile)
{
	std::map<int, std::shared_ptr<Resource>>::iterator it = resources.begin();

	for (; it != resources.end(); ++it)
	{
		if ((*it).second.get()->HasMetaFile())
		{
			//(*it).second.get().LoadMetaFile();
			(*it).second.get()->LoadMetaDataFile();

			return (*it).second;
		}
	}

	return nullptr;
}

//std::shared_ptr<Resource> ResourceManager::LoadMetaDataFile(const char* assetsFile, ResourceType type)
//{
//
//	switch (type)
//	{
//		case ResourceType::MODEL:
//		{
//			return LoadModelMetaData(assetsFile);
//			break;
//		}
//		case ResourceType::TEXTURE:
//		{
//			return LoadTextureMetaData(assetsFile);
//			break;
//		}
//	}
//
//	return std::shared_ptr<Resource>();
//}

std::shared_ptr<KbModel> ResourceManager::LoadModelMetaData(const char* assetsFile)
{
	std::shared_ptr<KbModel> ret = nullptr;

	char* buffer;
	if (app->fileSystem->Load(assetsFile, &buffer))
	{
		JSON_Value* value = json_parse_string(buffer);
		JSON_Object* obj = json_value_get_object(value);

		JSON_Array* arr = json_object_get_array(obj, "meshes");

		for (int i = 0; i < json_array_get_count(arr); ++i)
		{
			JSON_Object* meshObj = json_array_get_object(arr, i);
			std::string meshName = json_object_get_string(meshObj, "mesh name");
			std::string meshLibPath = json_object_get_string(meshObj, "mesh library path");
			//std::string meshAssetsPath = json_object_get_string(meshObj, "mesh assets path");
			if (KbMesh* m = MeshLoader::GetInstance()->LoadMeshCustomFormat(meshLibPath))
			{
				KbModel* model = new KbModel();
				model->uuid = json_object_get_number(obj, "model uuid");
				ret = std::make_shared<KbModel>(*model);
				
				m->SetName(meshName);
				//m->SetAssetsPath(meshAssetsPath.c_str());
				m->SetLibraryPath(meshLibPath);
				ret.get()->AddMesh(m);
			}
			else
			{
				// TODO: Could be possible to generate the resource again if the file from library gets deleted?
				//ret = CreateNewResource(meshAssetsPath.c_str(), ResourceType::MODEL);
				//KbModel* model = (KbModel*)ret.get();
				//MeshLoader::GetInstance()->SaveModelCustomFormat();
			}
		}
	}

	return ret;

}

std::shared_ptr<Resource> ResourceManager::LoadTextureMetaData(const char* assetsFile)
{
	return std::shared_ptr<Resource>();
}
