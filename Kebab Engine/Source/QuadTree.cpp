#include "QuadTree.h"

#include "GameObject.h"

#include <queue>

#include "mmgr/mmgr.h"

QuadNode::QuadNode()
{
	uperNode = nullptr;
	leaf = true;
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

void QuadTree::Insert(GameObject* go)
{
	std::queue<QuadNode*> que;

	que.push(root);

	while (!que.empty())
	{
		QuadNode* itNode = que.front();
		que.pop();

		if (itNode->section.Intersects(*go->GetGlobalAABB()) || itNode->section.Contains(*go->GetGlobalAABB()))
		{
			if (itNode->bucket.size() < MAX_BUCKET && itNode->leaf)
			{
				itNode->bucket.push_back(go);
			}
			else if (itNode->bucket.size() >= MAX_BUCKET && itNode->leaf)
			{
				Divide(itNode);

				for (std::vector<QuadNode*>::iterator it = itNode->underNodes.begin(); it != itNode->underNodes.end(); it++)
				{
					que.push((*it));
				}
			}
			else if (itNode->leaf == false)
			{
				for (std::vector<QuadNode*>::iterator it = itNode->underNodes.begin(); it != itNode->underNodes.end(); it++)
				{
					que.push((*it));
				}
			}
		}
	}
}

void QuadTree::Remove(GameObject* go)
{
	std::queue<QuadNode*> que;

	que.push(root);

	while (!que.empty())
	{
		QuadNode* itNode = que.front();
		que.pop();

		if (itNode->leaf == false)
		{
			for (std::vector<QuadNode*>::iterator it = itNode->underNodes.begin(); it != itNode->underNodes.end(); it++)
			{
				que.push((*it));
			}
		}
		else
		{
			for (std::vector<GameObject*>::iterator itGO = itNode->bucket.begin(); itGO != itNode->bucket.end(); itGO++)
			{
				if (itNode->section.Intersects(*(*itGO)->GetGlobalAABB()) || itNode->section.Contains(*(*itGO)->GetGlobalAABB()))
				{
					itNode->bucket.erase(itGO);
				}
			}
		}

		//if ()
	}
}

void QuadTree::Divide(QuadNode* node)
{
	float3 minPoint, maxPoint;

	QuadNode* nod1 = new QuadNode();
	nod1->uperNode = node;
	minPoint = node->section.minPoint;
	maxPoint = { node->section.CenterPoint().x, node->section.MaxY(), node->section.CenterPoint().z };
	nod1->section.Enclose(minPoint, maxPoint);

	QuadNode* nod2 = new QuadNode();
	nod2->uperNode = node;
	minPoint = { node->section.MinX(), node->section.MinY(), node->section.CenterPoint().z };
	maxPoint = { node->section.CenterPoint().x, node->section.MaxY(), node->section.MaxZ() };
	nod2->section.Enclose(minPoint, maxPoint);

	QuadNode* nod3 = new QuadNode();
	nod3->uperNode = node;
	minPoint = { node->section.CenterPoint().x, node->section.MinY(), node->section.MinZ() };
	maxPoint = { node->section.MaxX(), node->section.MaxY(), node->section.CenterPoint().z };
	nod3->section.Enclose(minPoint, maxPoint);

	QuadNode* nod4 = new QuadNode();
	nod4->uperNode = node;
	minPoint = { node->section.CenterPoint().x, node->section.MinY(), node->section.CenterPoint().z };
	maxPoint = node->section.maxPoint;
	nod4->section.Enclose(minPoint, maxPoint);

	for (std::vector<GameObject*>::iterator itGO = node->bucket.begin(); itGO != node->bucket.end(); itGO++)
	{
		for (std::vector<QuadNode*>::iterator itUN = node->underNodes.begin(); itUN != node->underNodes.end(); itUN++)
		{
			if ((*itUN)->section.Intersects(*(*itGO)->GetGlobalAABB()) || (*itUN)->section.Contains(*(*itGO)->GetGlobalAABB()))
			{
				(*itUN)->bucket.push_back((*itGO));
			}
		}
	}

	node->leaf = false;
	node->bucket.clear();
}

void QuadTree::RecalculateNode(QuadNode* node)
{
	if (node->uperNode != nullptr)
	{
		int numLeafs = 0;
		int numGO = 0;

		QuadNode* parent = node->uperNode;

		for (std::vector<QuadNode*>::iterator it = parent->underNodes.begin(); it != parent->underNodes.end(); it++)
		{
			if ((*it)->leaf)
			{
				numLeafs++;
				numGO += (*it)->bucket.size();
			}
			else
			{
				break;
			}
		}

		if (numLeafs == 4 && numGO <= MAX_BUCKET)
		{
			std::vector<GameObject*> gObj;

			for (std::vector<QuadNode*>::iterator it = parent->underNodes.begin(); it != parent->underNodes.end(); it++)
			{
				for (std::vector<QuadNode*>::iterator it = parent->underNodes.begin(); it != parent->underNodes.end(); it++)
				{

				}
			}
		}
	}
}

void QuadTree::RecalculateFromGO(GameObject* go)
{
}

void QuadTree::DrawTree()
{
}

bool QuadTree::Intersect(std::vector<GameObject*>& bucket)
{
	return false;
}
