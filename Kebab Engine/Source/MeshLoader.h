//#pragma once
//
//#include "Mesh.h"
//
//#include "assimp/cimport.h"
//#include "assimp/scene.h"
//#include "assimp/postprocess.h"
//
//#include <vector>
//
//class MeshLoader
//{
//public:
//	static MeshLoader* GetInstance();
//
//	virtual ~MeshLoader();
//
//	bool Start();
//	bool Update(float dt);
//	bool CleanUp();
//
//	// This loads every mesh in the scene (FBX), thur it returns a vector
//	std::vector<KbGeometry> LoadMesh(const char* filePath);
//	inline std::vector<KbGeometry> GetMeshes() { return meshes; }
//
//private:
//	MeshLoader();
//	MeshLoader(const MeshLoader&);
//	MeshLoader& operator=(const MeshLoader&);
//
//private:
//	static MeshLoader* instance;
//
//	KbMesh currentMesh;
//	std::vector<KbGeometry> meshes;
//};