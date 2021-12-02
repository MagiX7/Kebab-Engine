#pragma once

#include "Geometry/AABB.h"
#include "Geometry/Frustum.h"

#include <vector>

class GameObject;

#define MAX_BUCKET 10

class QuadNode
{
public:
	QuadNode();
	~QuadNode();

	QuadNode* uperNode;

	std::vector<QuadNode*> underNodes;
	std::vector<GameObject*> bucket;

	AABB section;

	bool leaf;
};

class QdTree
{
public:
	QdTree();
	~QdTree();

	void Create(AABB& limits);
	void CleanUp();

	void Insert(GameObject* go);
	void Remove(GameObject* go);

	void Divide(QuadNode* node);

	void Recalculate();

	void DrawTree();

	void Intersect(Frustum* frustum);

	QuadNode* GetRoot() { return root; }

private:
	
	QuadNode* root;
};