#pragma once

#include "Resource.h"

#include <vector>

class KbMesh;

class KbModel : public Resource
{
public:
	KbModel();
	virtual ~KbModel();

	void AddMesh(KbMesh* mesh);
	inline const std::vector<KbMesh*>& GetMeshes() { return meshes; }

	void CreateMetaDataFile(const char* assetsFile) override;
	void LoadMetaDataFile() override;

private:
	std::vector<KbMesh*> meshes;
};