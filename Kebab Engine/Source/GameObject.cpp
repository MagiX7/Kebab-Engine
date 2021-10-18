#include "GameObject.h"

#include "Globals.h"

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
	Component* ret = new Component(*this, type);

	components.push_back(ret);

	return ret;
}