#include "GameObject.h"

#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject(std::string name) : parent(nullptr), name(name)
{
	ComponentTransform* transform = (ComponentTransform*)CreateComponent(ComponentType::TRANSFORM);

	transform->Translate({ 0,0,0 });
	transform->Rotate({ 0,0,0 });
	transform->Scalate({ 1,1,1, });
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

// TODO: Should check if the component already exists
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

Component* GameObject::GetComponent(ComponentType type)
{
	for (const auto& comp : components)
	{
		if (comp->GetComponentType() == type) return comp;
	}

	return nullptr;
}

void GameObject::AddComponent(Component* comp)
{
	components.push_back(comp);
}

void GameObject::AddChild(GameObject* child)
{
	childs.push_back(child);
}