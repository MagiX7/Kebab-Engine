#include "Application.h"
#include "MeshLoader.h"

MeshLoader* MeshLoader::instance = nullptr;

MeshLoader* MeshLoader::GetInstance()
{
	if (!instance) instance = new MeshLoader();
	return instance;
}

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
	
	RELEASE(instance);

	return true;
}

std::vector<KebabGeometry*> MeshLoader::LoadMesh(const char* filePath)
{
	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];

			currentMesh.verticesCount = mesh->mNumVertices * 3;
			currentMesh.vertices = new float3[mesh->mNumVertices * 3];
			memcpy(currentMesh.vertices, mesh->mVertices, sizeof(float3) * mesh->mNumVertices);

			if (mesh->HasFaces())
			{
				currentMesh.indicesCount = mesh->mNumFaces * 3;
				currentMesh.indices = new uint32_t[mesh->mNumFaces * 3];
				
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

			if (mesh->HasNormals())
			{
				currentMesh.normalsCount = mesh->mNumVertices;
				currentMesh.normals = new float3[mesh->mNumVertices * 3];
				for (uint j = 0; j < mesh->mNumVertices; ++j)
				{
					memcpy(&currentMesh.normals[j * 3], mesh->mNormals, 3 * sizeof(float3));
				}
			}

			currentMesh.SetUpBuffers();
			currentMesh.vertexBuffer->AddData(currentMesh.normals, sizeof(float3) * currentMesh.normalsCount);
			meshes.push_back(&currentMesh);
			LOG("New Mesh loaded and added to the list");
		}
		aiReleaseImport(scene);
		return meshes;
	}
	else
		LOG(“Error loading scene % s”, file_path);
}