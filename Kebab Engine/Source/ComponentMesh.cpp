#include "ComponentMesh.h"

#include "ComponentTransform.h"

ComponentMesh::ComponentMesh(GameObject& compOwner)
{
	this->owner = &compOwner;
	this->type = ComponentType::TRANSFORM;
	this->active = true;
}

ComponentMesh::~ComponentMesh()
{}

void ComponentMesh::Enable()
{
	this->active = true;
}

void ComponentMesh::Disable()
{
	this->active = false;
}

void ComponentMesh::Update()
{

}