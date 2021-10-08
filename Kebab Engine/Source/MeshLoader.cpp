#include "Application.h"
#include "MeshLoader.h"

MeshLoader::MeshLoader()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

}

MeshLoader::~MeshLoader()
{
}

bool MeshLoader::Start()
{


	return true;
}

bool MeshLoader::Update(float dt)
{
	return true;
}

bool MeshLoader::CleanUp()
{
	meshes.clear();

	aiDetachAllLogStreams();
	
	return true;
}

KebabGeometry MeshLoader::LoadMesh(const char* filePath)
{
	mesh = KebabGeometry();

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* m = scene->mMeshes[i];

			vertices.resize(scene->mMeshes[i]->mNumVertices);
			vertices.push_back(scene->mMeshes[i]->mVertices->x);
			vertices.push_back(scene->mMeshes[i]->mVertices->y);
			vertices.push_back(scene->mMeshes[i]->mVertices->z);

			mesh.vertices = vertices;
			meshes.push_back(mesh);
			vertices.clear();

			if (m->HasFaces())
			{
				mesh.indices.resize(m->mNumFaces * 3);
				//ourMesh.indices = new uint[m.num_indices]; // assume each face is a triangle
				for (uint j = 0; j < m->mNumFaces; ++j)
				{
					if (m->mFaces[j].mNumIndices != 3)
						LOG("WARNING, geometry face with != 3 indices!");
					else
					{
						indices.resize(scene->mMeshes[i]->mNumFaces);
						indices[j] = (uint32_t)scene->mMeshes[i]->mFaces->mIndices[j];
						mesh.indices = indices;
						//memcpy(&m->mFaces->mIndices[j * 3], &mesh.indices, 3 * sizeof(uint32_t));
					}
					//memcpy(&m.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}
		}
		aiReleaseImport(scene);

		return mesh;
	}
	else
		LOG(“Error loading scene % s”, file_path);
}