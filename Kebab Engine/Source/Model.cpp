#include "Model.h"

KbModel::KbModel() : Resource(ResourceType::MODEL)
{
}

KbModel::~KbModel()
{
}

void KbModel::AddMesh(KbMesh* mesh)
{
	meshes.push_back(mesh);
}