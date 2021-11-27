#pragma once

#include "KbMesh.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

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

	GameObject* LoadModel(const std::string& path, bool loadOnScene = false);

	GameObject* LoadKbGeometry(KbGeometryType type);

	void SaveMeshCustomFormat(KbMesh* mesh, const std::string& name);
	void SaveMeshCustomFormat(ComponentMesh* mesh);
	KbMesh* LoadMeshCustomFormat(const std::string& fileName, GameObject* parent);

	void SaveModelCustomFormat(KbModel* model);
	GameObject* LoadModelCustomFormat(const std::string& filename);

	//GameObject* LoadPrimitive(PrimitiveType type);

	void CleanUp();

private:
	MeshLoader();
	MeshLoader(const MeshLoader&);

	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* go, const std::string& nameBaseGO, const std::string& path, KbModel* model);
	ComponentMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* baseGO, const std::string& nameBaseGO, const std::string& path, KbModel* model);
	//std::vector<Tex> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:


private:

	static MeshLoader* instance;

	std::vector<KbMesh> meshes;
	std::string directory;

	JSON_Value* modelValue;
};