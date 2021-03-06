#include "Application.h"
#include "GameObject.h"

#include "MainScene.h"

#include "Globals.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"

#include "QdTree.h"

#include <queue>

#include "mmgr/mmgr.h"

GameObject::GameObject(std::string name, int uuid) : parent(nullptr), name(name)
{
	ComponentTransform* transform = (ComponentTransform*)CreateComponent(ComponentType::TRANSFORM);

	transform->SetTranslation({ 0,0,0 });
	transform->SetRotation({ 0,0,0,1 });
	transform->SetScale({ 1,1,1, });

	//components.push_back(transform);

	active = true;

	insideFrustum = false;
	isStatic = false;
	inQT = false;

	if (uuid == 0)
	{
		LCG lgc = LCG();
		this->uuid = lgc.IntFast();
	}
	else this->uuid = uuid;
}

GameObject::~GameObject()
{
	parent = nullptr;

	app->scene->rootQT->Remove(this);
	app->scene->rootQT->Recalculate();

	for (auto& c : components)
	{
		delete(c);
		c = nullptr;
	}
	components.clear();

	for (auto& go : childs)
	{
		app->scene->rootQT->Remove(go);
		app->scene->rootQT->Recalculate();

		delete(go);
		go = nullptr;
	}
	childs.clear();
}

void GameObject::Update(float dt)
{
	std::queue<GameObject*> q;
	q.push(this);
	
	for (auto& go : childs)
		q.push(go);

	while (!q.empty())
	{
		GameObject* curr = q.front();
		q.pop();

		for (auto& comp : curr->components)
		{
			comp->Update(dt);
		}

		for (auto& child : curr->GetChilds())
			q.push(child);

	}
}

Component* GameObject::CreateComponent(ComponentType type, std::string meshPath)
{
	Component* ret = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		ret = new ComponentTransform(this);
		components.push_back(ret);
		break;

	case ComponentType::MESH:
		ret = new ComponentMesh(this, meshPath);
		components.push_back(ret);
		break;

	case ComponentType::MATERIAL:
		ret = new ComponentMaterial(this);
		components.push_back(ret);
		break;

	case ComponentType::CAMERA:
		ret = new ComponentCamera(this, CameraType::NONE);
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
		if (comp->GetComponentType() == type) return comp;

	return nullptr;
}

void GameObject::UnParent()
{
	if (parent)
	{
		for (std::vector<GameObject*>::iterator itChilds = parent->childs.begin(); itChilds != parent->childs.end(); itChilds++)
		{
			if ((*itChilds) == this)
			{
				parent->childs.erase(itChilds);
				parent->childs.shrink_to_fit();
				break;
			}
		}
	}
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
}

void GameObject::AddAABB()
{
	ComponentMesh* mesh = (ComponentMesh*)this->GetComponent(ComponentType::MESH);

	if (mesh)
	{
		localAABB.SetNegativeInfinity();
		for (uint i = 0; i < mesh->GetMesh()->vertices.size(); i++)
		{
			localAABB.Enclose(mesh->GetMesh()->vertices[i].position);
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

void GameObject::SetGlobalAABB(const AABB& aabb)
{
	localAABB = aabb;
	ComponentTransform* tr = (ComponentTransform*)GetComponent(ComponentType::TRANSFORM);
	localAABB.Transform(tr->GetGlobalMatrix());

	globalAABB.Enclose(localAABB);

	if (parent && parent != app->scene->GetRoot())
	{
		parent->SetGlobalAABB(globalAABB);
	}
	
	UpdateAABB(tr->GetGlobalMatrix());
}

AABB* GameObject::GetLocalAABB()
{
	return &localAABB;
}

AABB* GameObject::GetGlobalAABB()
{
	return &globalAABB;
}

void GameObject::UpdateAABB(float4x4& newTrans)
{
	OBB obb = localAABB.ToOBB();
	obb.Transform(newTrans);
	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(obb);
}

void GameObject::HasMoved()
{
	app->scene->rootQT->Recalculate();
}

void GameObject::PropagateTransform()
{
	for (uint i = 0; i < childs.size(); i++)
	{
		ComponentTransform* tr = (ComponentTransform*)childs[i]->GetComponent(ComponentType::TRANSFORM);
		tr->TransformParentMoved();
	}
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
		if (GameObject* p = app->scene->GetGameObjectByUuid(parentUuid))
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
		JSON_Object* compObj = json_array_get_object(compsArray, j);
		int type = json_object_get_number(compObj, "Type");
		
		if (type == 0)
		{
			ComponentTransform* trans = (ComponentTransform*)parent->GetComponent(ComponentType::TRANSFORM);
			trans->Load(compObj, parent);
		}
		else if (type == 1)
		{
			std::string p = json_object_get_string(compObj, "mesh library path");
			ComponentMesh* m = new ComponentMesh(parent, p.c_str());
			m->Load(compObj, parent);
			parent->AddComponent(m);
		}
		else if (type == 2)
		{
			ComponentMaterial* mat = new ComponentMaterial(parent);
			mat->Load(compObj, parent);
			parent->AddComponent(mat);

		}
		else if (type == 4)
		{
			ComponentCamera* cam = new ComponentCamera(parent, CameraType::GAME);
			cam->Load(compObj, parent);
			parent->AddComponent(cam);
		}
		else if (type == 5)
		{
			ComponentLight* l = new ComponentLight();
			l->SetParent(parent);
			l->Load(compObj, parent);
			parent->AddComponent(l);
		}
	}
}
