#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject& compOwner)
{
	this->owner = &compOwner;
	this->type = ComponentType::TRANSFORM;
	this->active = true;
}

ComponentTransform::~ComponentTransform()
{}

void ComponentTransform::Enable()
{
	this->active = true;
}

void ComponentTransform::Disable()
{
	this->active = false;
}

void ComponentTransform::Update()
{
	
}