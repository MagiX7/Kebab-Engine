#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject& compOwner)
{
	this->parent = &compOwner;
	this->type = ComponentType::MATERIAL;
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