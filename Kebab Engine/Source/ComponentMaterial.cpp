#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject& compOwner)
{
	this->owner = &compOwner;
	this->type = ComponentType::TRANSFORM;
	this->active = true;
}

ComponentMaterial::~ComponentMaterial()
{}

void ComponentMaterial::Enable()
{
	this->active = true;
}

void ComponentMaterial::Disable()
{
	this->active = false;
}

void ComponentMaterial::Update()
{

}