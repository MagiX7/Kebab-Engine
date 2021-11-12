#include "QuadTree.h"

#include "GameObject.h"

#include "mmgr/mmgr.h"

QuadNode::QuadNode()
{
	uperNode = nullptr;
	depth = 0;
}

QuadNode::~QuadNode()
{
	delete uperNode;

	for (QuadNode* node : underNodes)
	{
		RELEASE(node);
	}
}

QuadTree::QuadTree()
{
	root = new QuadNode();
}

QuadTree::~QuadTree()
{
	delete root;
}

void QuadTree::Create(AABB& limits)
{
	root->section = limits;
}

void QuadTree::Clear()
{

}

bool QuadTree::Insert(GameObject* go)
{
	QuadNode* node = root;
	
	bool theNode = false;

	if (go->GetGlobalAABB()->IsFinite())
	{
		while (node != nullptr)
		{
			if (node->section.Intersects(*go->GetGlobalAABB()))
			{
				if (node->bucket.size() > MAX_BUCKET)
				{
					if (node->underNodes.size() == 0)
						Divide(node);

					std::vector<QuadNode*>::iterator it;
					for (it = node->underNodes.begin(); it != node->underNodes.end(); it++)
					{
						if ((*it)->section.Intersects(*go->GetGlobalAABB()))
						{
							if ((*it)->bucket.size() <= MAX_BUCKET)
								(*it)->bucket.push_back(go);
						}
					}

					return true;
				}
				else if (node->bucket.size() <= MAX_BUCKET)
				{
					node->bucket.push_back(go);
					return true;
				}
			}
		}
	}

	return false;
}

bool QuadTree::Remove(Gameobject* go)
{
	bool removed = false;



	return removed;
}

void QuadTree::Divide(QuadNode* node)
{
	float3 minPoint, maxPoint;

	QuadNode* nod1 = new QuadNode();
	nod1->depth = node->depth + 1;
	minPoint = node->section.minPoint;
	maxPoint = { node->section.CenterPoint().x, node->section.MaxY(), node->section.CenterPoint().z };
	nod1->section.Enclose(minPoint, maxPoint);

	QuadNode* nod2 = new QuadNode();
	nod2->depth = node->depth + 1;
	minPoint = { node->section.MinX(), node->section.MinY(), node->section.CenterPoint().z };
	maxPoint = { node->section.CenterPoint().x, node->section.MaxY(), node->section.MaxZ() };
	nod2->section.Enclose(minPoint, maxPoint);

	QuadNode* nod3 = new QuadNode();
	nod3->depth = node->depth + 1;
	minPoint = { node->section.CenterPoint().x, node->section.MinY(), node->section.MinZ() };
	maxPoint = { node->section.MaxX(), node->section.MaxY(), node->section.CenterPoint().z };
	nod3->section.Enclose(minPoint, maxPoint);

	QuadNode* nod4 = new QuadNode();
	nod4->depth = node->depth + 1;
	minPoint = { node->section.CenterPoint().x, node->section.MinY(), node->section.CenterPoint().z };
	maxPoint = node->section.maxPoint;
	nod4->section.Enclose(minPoint, maxPoint);
}

void QuadTree::Calculate()
{
}

void QuadTree::DrawTree()
{
}

bool QuadTree::Intersect(std::vector<Gameobject*>& bucket)
{
	return false;
}
