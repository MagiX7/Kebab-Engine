#include "GameObject.h"

#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	for (auto& c: components)
	{
		RELEASE(c);
	}

	components.clear();
}

void GameObject::Update(float dt)
{

}

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* ret = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		ret = new ComponentTransform(*this);
		components.push_back(ret);
		break;
	case ComponentType::MESH:
		ret = new ComponentMesh(*this);
		components.push_back(ret);
		break;
	case ComponentType::MATERIAL:
		ret = new ComponentMaterial(*this);
		components.push_back(ret);
		break;
	default:
		break;
	}

	return ret;
}