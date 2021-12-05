#pragma once

#include "Resource.h"
#include "ModelProperties.h"

#include <vector>

class KbMesh;

class KbModel : public Resource
{
public:
	KbModel();
	virtual ~KbModel();

	void AddMesh(KbMesh* mesh);
	void AddTexture(Texture* tex);
	inline const std::vector<KbMesh*>& GetMeshes() { return meshes; }
	inline const std::vector<Texture*>& GeTextures() { return textures; }
	void DeleteMeshes();

	inline void SetProperties(const ModelProperties& props) { this->props = props; }

	void CreateMetaDataFile(const char* assetsFile) override;
	void LoadMetaDataFile() override;

private:
	std::vector<KbMesh*> meshes;
	std::vector<Texture*> textures;

	ModelProperties props;
};