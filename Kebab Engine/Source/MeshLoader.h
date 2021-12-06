#pragma once

#include "KbMesh.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModelProperties.h"

#include <assimp/scene.h>

class KbModel;

enum class KbGeometryType
{
	NONE = -1,
	CUBE,
	PYRAMID,
	PLANE,
	SPHERE,
	CYLINDER
};

class MeshLoader
{
public:
	static MeshLoader* GetInstance();

	virtual ~MeshLoader();

	GameObject* LoadModel(const std::string& path, bool loadOnScene = false, const ModelProperties& props = ModelProperties());

	GameObject* LoadKbGeometry(KbGeometryType type);

	void SaveMeshCustomFormat(KbMesh* mesh, const std::string& name, int uuid);
	void SaveMeshCustomFormat(ComponentMesh* mesh, int uuid);
	KbMesh* LoadMeshCustomFormat(const std::string& fileName);

	void SaveModelCustomFormat(GameObject* go, int modelUuid);
	GameObject* LoadModelCustomFormat(const std::string& filename, std::shared_ptr<KbModel> model = nullptr);

	void ReLoadModel(const char* path, const ModelProperties& props);

	void CleanUp();

private:
	MeshLoader();
	MeshLoader(const MeshLoader&);

	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* go, const std::string& nameBaseGO, const std::string& path, std::shared_ptr<KbModel> model);
	ComponentMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* baseGO, const std::string& nameBaseGO, const std::string& path, std::shared_ptr<KbModel> model);

	void ReProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<KbModel> model);
	void ReProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<KbModel> model);

	unsigned int GetModelFlags(const ModelProperties& props);

private:

	static MeshLoader* instance;

	// Only for reimporting
	std::vector<std::string> texturesMetaPath;

	std::vector<KbMesh> meshes;
	std::string directory;

	JSON_Value* modelValue;
};