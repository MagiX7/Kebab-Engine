#pragma once

#include "Geometry.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>

class MeshLoader
{
public:
	static MeshLoader* GetInstance();

	virtual ~MeshLoader();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	// This loads every mesh in the scene (FBX), thur it returns a vector
	std::vector<KebabGeometry*> LoadMesh(const char* filePath);
	inline std::vector<KebabGeometry*> GetMeshes() { return meshes; }

private:
	MeshLoader();
	MeshLoader(const MeshLoader&);
	MeshLoader& operator=(const MeshLoader&);

private:
	static MeshLoader* instance;

	KebabGeometry currentMesh;
	std::vector<KebabGeometry*> meshes;
};