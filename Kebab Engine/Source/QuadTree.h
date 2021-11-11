#pragma once

#include "Geometry/AABB.h"

#include <vector>

class Gameobject;

#define MAX_BUCKET 5

class QuadNode
{
public:
	QuadNode();
	~QuadNode();

	QuadNode* uperNode;

	std::vector<QuadNode*> underNodes;
	std::vector<Gameobject*> bucket;

	AABB section;

	int depth;
};

class QuadTree
{
public:
	QuadTree();
	~QuadTree();

	void Create(AABB& limits);

	void Clear();

	bool Insert(Gameobject* go);
	bool Remove(Gameobject* go);

	void Divide(QuadNode* node);

	void Calculate();

	void DrawTree();

	bool Intersect(std::vector<Gameobject*>& bucket);

private:
	
	QuadNode* root;
};