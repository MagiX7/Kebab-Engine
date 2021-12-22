#pragma once

struct ModelProperties
{
	bool joinIdenticalVertices = false;
	// All faces are triangles
	bool triangulate = true;
	bool genNormals = true;
	bool genSmoothNormals = false;
	bool removeRedundantMaterials = false;
	bool fixInfacingNormals = false;
	bool genUVCoords = true;
	bool transformUVCoords = true;
	// Replaces exact same meshes with a reference to the first one
	bool findInstances = true;
	// Reduce the number of meshes
	bool optimizeMeshes = true;
	bool flipUVs = true;
};