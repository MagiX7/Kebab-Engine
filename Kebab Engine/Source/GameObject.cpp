#include "GameObject.h"

#include "Application.h"
#include "MainScene.h"

#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include <queue>

#include "mmgr/mmgr.h"

GameObject::GameObject(std::string name, int uuid) : parent(nullptr), name(name)
{
	ComponentTransform* transform = (ComponentTransform*)CreateComponent(ComponentType::TRANSFORM);

	transform->SetTranslation({ 0,0,0 });
	transform->SetRotation({ 0,0,0,1 });
	transform->SetScale({ 1,1,1, });

	//localAABB = nullptr;
	localAABB = AABB::AABB();
	active = true;

	if (uuid == 0)
	{
		LCG lgc = LCG();
		this->uuid = lgc.IntFast();
	}
}

GameObject::~GameObject()
{
	//DeleteChilds(this);

	for (auto& go : childs)
	{
		delete(go);
		go = nullptr;
	}
	childs.clear();

	for (auto& c: components)
	{
		delete(c);
		c = nullptr;
	}
	components.clear();

	childs.clear();
	components.clear();

	//RELEASE(localAABB);
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
	case ComponentType::CAMERA:
		ret = new ComponentCamera(*this);
		components.push_back(ret);
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

void GameObject::EraseChild(GameObject* go)
{
	std::vector<GameObject*>::iterator it = childs.begin();

	for (; it != childs.end(); ++it)
	{
		if (*it == go)
		{
			childs.erase(it);
			childs.shrink_to_fit();

			LOG_CONSOLE("Deleted GameObject %s from %s", go->GetName().c_str(), go->GetParent()->GetName().c_str());

			delete go;
			go = nullptr;
			break;
		}
	}
}

void GameObject::AddComponent(Component* comp)
{
	components.push_back(comp);

	AddAABB();
}

void GameObject::AddChild(GameObject* child)
{
	childs.push_back(child);
	SetGlobalAABB(this);
}

void GameObject::AddAABB()
{
	ComponentMesh* mesh = (ComponentMesh*)this->GetComponent(ComponentType::MESH);

	if (mesh != nullptr)
	{
		//if (!localAABB)
		//{
		//	localAABB = new AABB();
		//}

		localAABB.SetNegativeInfinity();
		for (uint i = 0; i < mesh->vertices.size(); i++)
		{
			localAABB.Enclose(mesh->vertices[i].position);
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

void GameObject::SetGlobalAABB(GameObject* p)
{
	if (childs.size() != 0)
	{
		p->localAABB.SetNegativeInfinity();

		for (uint i = 0; i < childs.size(); i++)
		{
			childs[i]->SetGlobalAABB(p);
		}
	}

	p->localAABB.Enclose(localAABB.minPoint, localAABB.maxPoint);
}

AABB* GameObject::GetLocalAABB()
{
	return &localAABB;
}

AABB* GameObject::GetGlobalAABB()
{
	ComponentTransform* compTrans = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);
	OBB obb = localAABB.Transform(compTrans->GetLocalMatrix());
	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(obb);
	return &globalAABB;
}

void GameObject::UpdateAABB(float4x4& newTrans)
{
	OBB obb = localAABB.Transform(newTrans);
	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(obb);
}

void GameObject::Save(JSON_Array* array)
{
	JSON_Value* value = Parser::InitValue();
	JSON_Object* goObj = Parser::GetObjectByValue(value);

	Parser::SetObjectString(goObj, "name", name.c_str());
	Parser::SetObjectNumber(goObj, "uuid", uuid);
	if (parent) Parser::SetObjectNumber(goObj, "parent uuid", parent->uuid);

	Parser::AppendValueToArray(array, value);

	if (childs.size() > 0)
		for (int i = 0; i < childs.size(); ++i)
			childs[i]->Save(array);


	// Components
	JSON_Value* arrValue = Parser::InitArray();
    JSON_Array* compsArray = Parser::GetArrayByValue(arrValue);

    Parser::DotSetObjectValue(goObj, "Components", arrValue);

	for (const auto& comp : components)
		Parser::AppendValueToArray(compsArray, comp->Save());
}

JSON_Value* GameObject::Load(JSON_Object* obj)
{
	uuid = json_object_get_number(obj, "uuid");
	int parentUuid = json_object_get_number(obj, "parent uuid");
	if (parentUuid)
	{
		GameObject* p = app->scene->GetGameObjectByUuid(parentUuid);
		if (p)
		{
			p->childs.push_back(this);
			parent = p;
		}
	}

	JSON_Array* comps = json_object_dotget_array(obj, "Components");
	LoadComponents(comps, this);

	return nullptr;
}

void GameObject::LoadComponents(JSON_Array* compsArray, GameObject* parent)
{
	for (int j = 0; j < json_array_get_count(compsArray); ++j)
	{
		Component* comp = nullptr;
		JSON_Object* compObj = json_array_get_object(compsArray, j);
		int type = json_object_get_number(compObj, "Type");
		
		if (type == 0)
			comp = new ComponentTransform(*parent);
		else if (type == 1)
		{
			std::string p = json_object_get_string(compObj, "path");
			comp = new ComponentMesh(*parent, p.c_str());

		}
		else if (type == 2)
			comp = new ComponentMaterial(*parent);
		else if (type == 3)
			comp = new ComponentCamera(*parent);

		if (comp)
		{
			parent->AddComponent(comp);
			comp->Load(compObj, parent);
		}
	}
}
