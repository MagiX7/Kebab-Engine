#pragma once

#include "VertexArray.h"
#include "Texture.h"

#include "Vertex.h"

struct Tex
{
	unsigned int id;
	std::string type;
	std::string path;
};

class KbMesh
{
public:
	KbMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures, std::vector<float2> texCoords);
	//KbMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* texture, const char* fileName);
	//KbMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const char* fileName);
	virtual ~KbMesh();

	void BeginDraw();
	void Draw(bool showNormals);
	void EndDraw();

private:
	void SetUpMesh();

public:
	std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture>  textures;

private:
	//VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Texture* texture;

	GLuint texBuffer;

};