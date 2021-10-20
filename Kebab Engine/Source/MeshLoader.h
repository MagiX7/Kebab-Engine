#pragma once

#include "Mesh.h"
#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include <assimp/scene.h>

class MeshLoader
{
public:
	static MeshLoader* GetInstance();

	virtual ~MeshLoader();

	GameObject* LoadModel(std::string path);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, GameObject* go);
	//Component* ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* baseGO);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, GameObject* baseGO);
	std::vector<Tex> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	MeshLoader();
	MeshLoader(const MeshLoader&);

	static MeshLoader* instance;

	std::vector<KbMesh> meshes;
	std::string directory;
};