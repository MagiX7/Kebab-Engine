#include "Application.h"
#include "QdTree.h"

#include "GameObject.h"
#include "Renderer3D.h"

#include "Geometry/Plane.h"

#include <queue>

#include "mmgr/mmgr.h"

QuadNode::QuadNode()
{
	uperNode = nullptr;
	leaf = true;
}

QuadNode::~QuadNode()
{
	for (QuadNode* node : underNodes)
	{
		RELEASE(node);
	}
}

QdTree::QdTree()
{
	root = new QuadNode();
}

QdTree::~QdTree()
{
	delete root;
}

void QdTree::Create(AABB& limits)
{
	root->section = limits;
}

void QdTree::Insert(GameObject* go)
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

void QdTree::Remove(GameObject* go)
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
				if ((*itGO) = go)
				{
					itNode->bucket.erase(itGO);
					break;
				}
			}
		}
	}
}

void QdTree::Divide(QuadNode* node)
{
	float3 minPoint, maxPoint;
	float3 centerP = node->section.CenterPoint();

	QuadNode* nod1 = new QuadNode();
	nod1->uperNode = node;
	minPoint = node->section.minPoint;
	maxPoint = { centerP.x, node->section.MaxY(), centerP.z };
	nod1->section.minPoint = minPoint;
	nod1->section.maxPoint = maxPoint;

	QuadNode* nod2 = new QuadNode();
	nod2->uperNode = node;
	minPoint = { node->section.MinX(), node->section.MinY(), centerP.z };
	maxPoint = { centerP.x, node->section.MaxY(), node->section.MaxZ() };
	nod2->section.minPoint = minPoint;
	nod2->section.maxPoint = maxPoint;

	QuadNode* nod3 = new QuadNode();
	nod3->uperNode = node;
	minPoint = { centerP.x, node->section.MinY(), node->section.MinZ() };
	maxPoint = { node->section.MaxX(), node->section.MaxY(), centerP.z };
	nod3->section.minPoint = minPoint;
	nod3->section.maxPoint = maxPoint;

	QuadNode* nod4 = new QuadNode();
	nod4->uperNode = node;
	minPoint = { centerP.x, node->section.MinY(),centerP.z };
	maxPoint = node->section.maxPoint;
	nod4->section.minPoint = minPoint;
	nod4->section.maxPoint = maxPoint;

	node->underNodes.push_back(nod1);
	node->underNodes.push_back(nod2);
	node->underNodes.push_back(nod3);
	node->underNodes.push_back(nod4);

	for (std::vector<QuadNode*>::iterator itUN = node->underNodes.begin(); itUN != node->underNodes.end(); itUN++)
	{
		for (std::vector<GameObject*>::iterator itGO = node->bucket.begin(); itGO != node->bucket.end(); itGO++)
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

void QdTree::Recalculate()
{
	std::queue<QuadNode*> que;
	std::vector<GameObject*> gObjs;

	que.push(root);

	while (!que.empty())
	{
		QuadNode* itNode = que.front();
		que.pop();

		if (itNode->leaf == false)
		{
			for (std::vector<QuadNode*>::iterator itUN = itNode->underNodes.begin(); itUN != itNode->underNodes.end(); itUN++)
			{
				que.push((*itUN));
			}
		}
		else
		{
			for (std::vector<GameObject*>::iterator itGO = itNode->bucket.begin(); itGO != itNode->bucket.end(); itGO++)
			{
				bool repeated = false;

				for (std::vector<GameObject*>::iterator it = gObjs.begin(); it != gObjs.end(); it++)
				{
					if ((*itGO) == (*it))
					{
						repeated = true;
					}
				}

				if (repeated == false)
					gObjs.push_back((*itGO));
			}
		}
	}

	for (std::vector<GameObject*>::iterator itGO = gObjs.begin(); itGO != gObjs.end(); itGO++)
	{
		Remove((*itGO));
	}

	for (std::vector<QuadNode*>::iterator itUN = root->underNodes.begin(); itUN != root->underNodes.end(); itUN++)
	{
		delete((*itUN));
	}

	root->underNodes.clear();
	root->bucket.clear();
	root->leaf = true;

	for (std::vector<GameObject*>::iterator itGO = gObjs.begin(); itGO != gObjs.end(); itGO++)
	{
		Insert((*itGO));
	}
}

void QdTree::DrawTree()
{
	std::queue<QuadNode*> que;
	std::vector<GameObject*> gObjs;

	que.push(root);

	while (!que.empty())
	{
		QuadNode* itNode = que.front();
		que.pop();

		for (std::vector<QuadNode*>::iterator itUN = itNode->underNodes.begin(); itUN != itNode->underNodes.end(); itUN++)
		{
			que.push((*itUN));
		}

		app->renderer3D->DrawAABB(itNode->section);
	}
}

bool QdTree::Intersect(Frustum* frustum)
{


	return false;
}
