#include "Application.h"
#include "FileSystem.h"

#include "MeshLoader.h"

#include "Model.h"
#include "KbMesh.h"

#include "parson.h"

KbModel::KbModel() : Resource(ResourceType::MODEL)
{
}

KbModel::~KbModel()
{
}

void KbModel::AddMesh(KbMesh* mesh)
{
	meshes.push_back(mesh);
}

void KbModel::CreateMetaDataFile(const char* assetsFile)
{
	hasMetaFile = true;

	std::string p = assetsFile;
	//int end = p.find(".");
	//std::string path = p.substr(0, end);
	p += ".meta";
	metaFile = p;

	JSON_Value* value = json_value_init_object();
	JSON_Object* obj = json_value_get_object(value);

	json_object_set_number(obj, "model uuid", uuid);

	JSON_Value* arrVal = json_value_init_array();
	JSON_Array* arr = json_value_get_array(arrVal);

	json_object_dotset_value(obj, "meshes", arrVal);
	for (int i = 0; i < meshes.size(); ++i)
	{
		JSON_Value* meshVal = json_value_init_object();
		JSON_Object* meshObj = json_value_get_object(meshVal);

		json_object_set_string(meshObj, "mesh name", meshes[i]->GetName().c_str());
		json_object_set_string(meshObj, "mesh library path", meshes[i]->GetLibraryPath().c_str());
		//json_object_set_string(meshObj, "mesh assets path", meshes[i]->GetAssetsPath().c_str());
		json_array_append_value(arr, meshVal);
	}

	json_serialize_to_file_pretty(value, p.c_str());
	json_value_free(value);
}

void KbModel::LoadMetaDataFile()
{
	for (auto& m : meshes)
	{
		delete m;
	}
	meshes.clear();

	char* buffer;
	if (app->fileSystem->Load(metaFile.c_str(), &buffer))
	{
		JSON_Value* value = json_parse_string(buffer);
		JSON_Object* obj = json_value_get_object(value);

		uuid = json_object_get_number(obj, "model uuid");

		JSON_Array* arr = json_object_get_array(obj, "meshes");

		for (int i = 0; i < json_array_get_count(arr); ++i)
		{
			JSON_Object* meshObj = json_array_get_object(arr, i);
			std::string meshName = json_object_get_string(meshObj, "mesh name");
			std::string meshPath = json_object_get_string(meshObj, "mesh library path");
			KbMesh* m = MeshLoader::GetInstance()->LoadMeshCustomFormat(meshPath);
			m->SetName(meshName);
			meshes.push_back(m);
		}
	}
}