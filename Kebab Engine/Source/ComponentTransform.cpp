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

void ComponentTransform::SetPosition(const float3& pos)
{
	position = pos;
}

void ComponentTransform::SetRotation(const Quat& rot)
{
	rotation = rot;
}

void ComponentTransform::SetScale(const float3& scal)
{
	scale = scal;
}