#pragma once

#include "Geometry.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>

class MeshLoader
{
public:
	MeshLoader();
	virtual ~MeshLoader();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	KebabGeometry* LoadMesh(const char* filePath);

private:

	KebabGeometry currentMesh;
	std::vector<KebabGeometry> meshes;
};