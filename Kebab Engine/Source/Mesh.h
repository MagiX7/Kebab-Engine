#pragma once

#include "VertexArray.h"
#include "Texture.h"

#include "Vertex.h"


//struct Vertex
//{
//	float3 position;
//	float3 normal;
//	float2 texCoords;
//};

struct Tex
{
	unsigned int id;
	std::string type;
	std::string path;
};

class KbMesh
{
public:
	KbMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Tex> textures);


	void BeginDraw();
	void Draw(bool showNormals);
	void EndDraw();

private:
	void SetUpMesh();

public:
	std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;
	std::vector<Tex>  textures;

private:
	//VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Texture* texture;
};