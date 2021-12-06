#include "Application.h"
#include "FileSystem.h"

#include "ResourceManager.h"
#include "Resource.h"

#include "MeshLoader.h"
#include "TextureLoader.h"

#include "Model.h"

#include <Algorithm/Random/LCG.cpp>

#include "mmgr/mmgr.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::GetInstance()
{
	if (!instance) instance = new ResourceManager();
	return instance;
}

void ResourceManager::CleanUp()
{
	for (auto r : resources)
		r.second.reset();

	resources.clear();

	delete instance;
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

}

void ResourceManager::AddResource(KbMesh* res)
{
	std::shared_ptr<Resource> resource = std::make_shared<KbMesh>(*res);
	resources[res->uuid] = resource;
}

void ResourceManager::AddResource(Texture* res)
{
	std::shared_ptr<Resource> resource = std::make_shared<Texture>(*res);
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
		if ((*it).second->GetAssetsPath() == assetsFile)
		{
			return (*it).second;
		}
	}

	return nullptr;
}

std::shared_ptr<Resource> ResourceManager::IsAlreadyLoaded(const std::string& libraryFile)
{
	std::map<int, std::shared_ptr<Resource>>::iterator it = resources.begin();

	for (; it != resources.end(); ++it)
	{
		if ((*it).second.get()->GetLibraryPath() == libraryFile)
		{
			return (*it).second;
		}
	}

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

std::shared_ptr<KbModel> ResourceManager::CreateModel(const char* assetsFile)
{
	std::shared_ptr<KbModel> ret = nullptr;

	KbModel* model = new KbModel();

	model->uuid = GenerateUUID();
	ret = std::make_shared<KbModel>(*model);

	ret->SetAssetsPath(assetsFile);
	std::string tmp = assetsFile;
	int start = tmp.find_last_of("/");
	int end = tmp.find(".");
	std::string lib = "Library/Models/" + tmp.substr(start + 1, end - start - 1) + "__" + std::to_string(model->uuid) + ".kbmodel";
	ret->SetLibraryPath(lib);

	resources[ret->uuid] = ret;
	
	return ret;
}

std::shared_ptr<Resource> ResourceManager::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::string& name)
{
	std::shared_ptr<Resource> ret = nullptr;



	return ret;
}

std::shared_ptr<Texture> ResourceManager::CreateTexture(const char* assetsFile, int modelUuid, const TextureProperties& props, int texUuid)
{
	std::shared_ptr<Texture> ret = nullptr;

	Texture* tex = TextureLoader::GetInstance()->LoadTexture(assetsFile, props);
	if (texUuid == -1)
		tex->uuid = GenerateUUID();
	else
		tex->uuid = texUuid;

	ret = std::make_shared<Texture>(*tex);

	ret->SetAssetsPath(assetsFile);
	std::string tmp = assetsFile;
	int start = tmp.find_last_of("/");
	int end = tmp.find(".");
	std::string lib = "Library/Textures/" + tmp.substr(start + 1, end - start - 1) +
		"__" + std::to_string(modelUuid) + ".kbtexture";
	ret->SetLibraryPath(lib);

	ret->CreateMetaDataFile(assetsFile);

	resources[ret->uuid] = ret;


	return ret;
}

std::shared_ptr<Resource> ResourceManager::LoadTexture(const char* libraryFile, int uuid)
{
	std::shared_ptr<Resource> ret = nullptr;

	if (ret = IsAlreadyLoaded(std::string(libraryFile)))
		return ret;

	Texture* tex = TextureLoader::GetInstance()->LoadTextureCustomFormat(libraryFile);
	tex->uuid = GenerateUUID();
	tex->SetLibraryPath(libraryFile);

	ret = std::make_shared<Texture>(*tex);

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
			(*it).second.get()->LoadMetaDataFile();

			return (*it).second;
		}
	}

	return nullptr;
}

std::shared_ptr<KbModel> ResourceManager::LoadModelMetaData(const char* assetsFile)
{
	std::shared_ptr<KbModel> ret = nullptr;

	char* buffer;
	if (app->fileSystem->Load(assetsFile, &buffer))
	{
		JSON_Value* value = json_parse_string(buffer);
		JSON_Object* obj = json_value_get_object(value);

		JSON_Array* arr = json_object_get_array(obj, "meshes");

		KbModel* model = new KbModel();
		model->uuid = json_object_get_number(obj, "model uuid");
		model->SetLibraryPath(json_object_get_string(obj, "model library path"));
		model->SetAssetsPath(json_object_get_string(obj, "model assets path"));

		int size = json_array_get_count(arr);
		for (int i = 0; i < size; ++i)
		{
			JSON_Object* meshObj = json_array_get_object(arr, i);
			std::string meshName = json_object_get_string(meshObj, "mesh name");
			std::string meshLibPath = json_object_get_string(meshObj, "mesh library path");
			if (KbMesh* m = MeshLoader::GetInstance()->LoadMeshCustomFormat(meshLibPath))
			{
				m->SetName(meshName);
				m->SetLibraryPath(meshLibPath);
				const char* metaPath = json_object_get_string(meshObj, "texture meta path");
				if (metaPath)
				{
					LoadTextureMetaData(metaPath);
					m->SetTextureMetaPath(metaPath);
				}
				model->AddMesh(m);
			}
			else
			{
				// TODO: Could be possible to generate the resource again if the file from library gets deleted?
				//ret = CreateNewResource(meshAssetsPath.c_str(), ResourceType::MODEL);
				//KbModel* model = (KbModel*)ret.get();
				//MeshLoader::GetInstance()->SaveModelCustomFormat();
			}
		}

		ModelProperties props;
		props.joinIdenticalVertices = json_object_dotget_boolean(obj, "join verts");
		props.triangulate = json_object_dotget_boolean(obj, "triangulate");
		props.genNormals = json_object_dotget_boolean(obj, "gen normals");
		props.genSmoothNormals = json_object_dotget_boolean(obj, "gen smooth normals");
		props.removeRedundantMaterials = json_object_dotget_boolean(obj, "rem mats");
		props.fixInfacingNormals = json_object_dotget_boolean(obj, "infacing normals");
		props.genUVCoords = json_object_dotget_boolean(obj, "gen uv coords");
		props.transformUVCoords = json_object_dotget_boolean(obj, "trans uv coords");
		props.findInstances = json_object_dotget_boolean(obj, "find instances");
		props.optimizeMeshes = json_object_dotget_boolean(obj, "opt meshes");
		props.flipUVs = json_object_dotget_boolean(obj, "flip uvs");
		model->SetProperties(props);

		ret = std::make_shared<KbModel>(*model);
		resources[ret->uuid] = ret;

		delete[] buffer;
	}

	return ret;

}

std::shared_ptr<Texture> ResourceManager::LoadTextureMetaData(const char* assetsFile)
{
	std::shared_ptr<Texture> ret = nullptr;
	TextureProperties props;

	std::string path = std::string(assetsFile);
	std::string assetsPath = assetsFile;
	if (path.find(".meta") == -1)
	{
		path += ".meta";
	}
	else if (assetsPath.find(".meta") != -1)
	{
		int s = assetsPath.find(".meta");
		assetsPath = assetsPath.substr(0, s);
	}

	if (ret = std::static_pointer_cast<Texture>(IsAlreadyLoaded(assetsPath.c_str())))
		return ret;
	

	char* buffer;
	if (app->fileSystem->Load(path.c_str(), &buffer))
	{
		JSON_Value* value = json_parse_string(buffer);
		JSON_Object* obj = json_value_get_object(value);

		props.compression = json_object_get_number(obj, "compression");
		props.mipmap = json_object_get_boolean(obj, "mipmap");
		props.gaussianBlur = json_object_get_boolean(obj, "gaussianBlur");
		if (props.gaussianBlur)
			props.gaussianBlurIterations = json_object_get_number(obj, "gaussianBlurIterations");
		props.averageBlur = json_object_get_boolean(obj, "averageBlur");
		if (props.averageBlur)
			props.averageBlurIterations = json_object_get_number(obj, "averageBlurIterations");

		props.contrast = json_object_get_boolean(obj, "contrast");
		if (props.contrast)
			props.contrastAmount = json_object_get_number(obj, "contrastAmount");

		props.alienify = json_object_get_boolean(obj, "alienify");
		props.equalization = json_object_get_boolean(obj, "equalization");
		props.gammaCorrection = json_object_get_boolean(obj, "gammaCorrection");
		if (props.gammaCorrection)
			props.gammaCorrectionAmount = json_object_get_number(obj, "gammaCorrectionAmount");

		props.negativity = json_object_get_boolean(obj, "negativity");
		props.noise = json_object_get_boolean(obj, "noise");
		if (props.noise)
			props.noiseAmount = json_object_get_number(obj, "noiseAmount");

		props.pixelization = json_object_get_boolean(obj, "pixelization");
		if (props.pixelization)
			props.pixelsSize = json_object_get_number(obj, "pixelsSize");

		props.sharpening = json_object_get_boolean(obj, "sharpening");
		if (props.sharpening)
		{
			props.sharpeningAmount = json_object_get_number(obj, "sharpeningAmount");
			props.sharpeningIterations = json_object_get_number(obj, "sharpeningIterations");
		}

		std::string libPath = json_object_get_string(obj, "texture library path");
		std::string assetsPath = json_object_get_string(obj, "texture assets path");

		ret = std::static_pointer_cast<Texture>(IsAlreadyLoaded(assetsPath.c_str()));
		if (!ret)
		{
			Texture* tex = TextureLoader::GetInstance()->LoadTextureCustomFormat(libPath, props);
			if (tex)
			{
				tex->uuid = json_object_get_number(obj, "texture uuid");
				tex->SetAssetsPath(assetsPath);
				tex->SetMetaPath(path);

				ret = std::make_shared<Texture>(*tex);

				resources[ret->uuid] = ret;
				ret->SetProperties(props);
			}
		}

		delete[] buffer;
	}

	return ret;
}
