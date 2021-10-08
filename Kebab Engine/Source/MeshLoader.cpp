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

KebabGeometry* MeshLoader::LoadMesh(const char* filePath)
{
	currentMesh = KebabGeometry();

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];

			currentMesh.vertices.resize(mesh->mNumVertices);
			/*currentMesh.vertices.push_back(scene->mMeshes[i]->mVertices->x);
			currentMesh.vertices.push_back(scene->mMeshes[i]->mVertices->y);
			currentMesh.vertices.push_back(scene->mMeshes[i]->mVertices->z);*/
			//memcpy(&currentMesh.vertices, &mesh->mVertices, sizeof(mesh->mNumVertices));

			for (int k = 0; k < mesh->mNumVertices; k += 3)
			{
				currentMesh.vertices[k] = mesh->mVertices[k].x;
				currentMesh.vertices[k + 1] = mesh->mVertices[k].y;
				currentMesh.vertices[k + 2] = mesh->mVertices[k].z;
			}

			if (mesh->HasFaces())
			{
				currentMesh.indices.resize(mesh->mNumFaces * 3);
				
				for (uint j = 0; j < mesh->mNumFaces; ++j)
				{
					if (mesh->mFaces[j].mNumIndices != 3)
						LOG("WARNING, geometry face with != 3 indices!");
					else
					{
						memcpy(&currentMesh.indices[j * 3], mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}

			currentMesh.SetUpBuffers();
			meshes.push_back(currentMesh);
			return &currentMesh;
		}
		aiReleaseImport(scene);
	}
	else
		LOG(“Error loading scene % s”, file_path);
}