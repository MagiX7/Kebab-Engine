#pragma once

#include "Mesh.h"
#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include <assimp/scene.h>

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

	GameObject* LoadModel(std::string path);

	GameObject* LoadKbGeometry(KbGeometryType type);

	void SaveMeshCustomFormat(ComponentMesh* mesh);
	ComponentMesh* LoadMeshCustomFormat(const char* fileName, GameObject* parent);


	//GameObject* LoadPrimitive(PrimitiveType type);

	void CleanUp();

private:
	MeshLoader();
	MeshLoader(const MeshLoader&);

	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* go, std::string nameBaseGO);
	ComponentMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* baseGO, std::string nameBaseGO);
	std::vector<Tex> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:


private:

	static MeshLoader* instance;

	std::vector<KbMesh> meshes;
	std::string directory;
};