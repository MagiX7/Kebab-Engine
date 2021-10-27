#include "GameObject.h"

#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject(std::string name) : parent(nullptr), name(name)
{
	ComponentTransform* transform = (ComponentTransform*)CreateComponent(ComponentType::TRANSFORM);

	transform->SetTranslation({ 0,0,0 });
	transform->SetRotation({ 0,0,0,1 });
	transform->SetScale({ 1,1,1, });

	localAABB = nullptr;
	active = true;
}

GameObject::~GameObject()
{
	for (auto& go : childs)
	{
		RELEASE(go);
	}
	childs.clear();

	for (auto& c: components)
	{
		RELEASE(c);
	}
	components.clear();

	RELEASE(localAABB);
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

void GameObject::UnParent()
{
	parent = nullptr;
	/*if(parent)
	{
		for (int i = 0; i < parent->GetChilds().size(); ++i)
		{
			if (parent->GetChilds()[i] == this)
			{
				parent.
			}
		}
	}*/
}

void GameObject::AddComponent(Component* comp)
{
	components.push_back(comp);

	AddAABB();
}

void GameObject::AddChild(GameObject* child)
{
	childs.push_back(child);
}

void GameObject::AddAABB()
{
	ComponentMesh* mesh = (ComponentMesh*)this->GetComponent(ComponentType::MESH);

	if (mesh != nullptr)
	{
		if (localAABB == nullptr)
		{
			localAABB = new AABB();
		}

		localAABB->SetNegativeInfinity();
		for (uint i = 0; i < mesh->vertices.size(); i++)
		{
			localAABB->Enclose(mesh->vertices[i].position);
		}
	}

	if (childs.size() != 0)
	{
		for (uint i = 0; i < childs.size(); i++)
		{
			childs[i]->AddAABB();
		}
	}
}

AABB* GameObject::GetLocalAABB()
{
	return localAABB;
}

void GameObject::SetCompleteAABB(GameObject* p)
{
	if (childs.size() != 0)
	{
		for (uint i = 0; i < childs.size(); i++)
		{
			childs[i]->SetCompleteAABB(p);
		}
	}

	if (localAABB != nullptr)
		p->aabb.Enclose(localAABB->minPoint, localAABB->maxPoint);
}

AABB* GameObject::GetCompleteAABB()
{
	return &aabb;
}

void GameObject::UpdateAABB(float4x4& newTrans)
{
	localAABB->TransformAsAABB(newTrans);
}
