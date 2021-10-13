#pragma once

#include "VertexArray.h"

#include "Vertex.h"

#include <Math/float3.h>
#include <Math/float2.h>

//struct Vertex
//{
//	float3 position;
//	float3 normal;
//	float2 texCoords;
//};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class KbMesh
{
public:
	KbMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

	void Draw();

private:
	void SetUpMesh();

public:
	std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture>  textures;

private:
	VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

};