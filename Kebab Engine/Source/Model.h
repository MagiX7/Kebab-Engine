#pragma once

#include "Mesh.h"
#include <assimp/scene.h>

class KbModel
{
public:
	KbModel(const char* path);
	virtual ~KbModel();

	void Draw(bool showNormals);

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	KbMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	std::vector<KbMesh> meshes;
	std::string directory;

	unsigned int vao;
};