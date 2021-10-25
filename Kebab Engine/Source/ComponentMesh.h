#pragma once

#include "Component.h"

#include "Texture.h"

#include "Vertex.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject& compOwner);
	~ComponentMesh();

	void Enable();
	void Disable();

	void Update();
	void Draw(bool drawVertexNormals = false, bool drawTriangleNormals = false);

	void SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* tex/*, std::vector<Texture> textures*/);
	void SetTexture(Texture* tex);

	void DrawOnInspector();
	void DrawVertexNormals();
	void DrawTriangleNormals();

private:
	void DrawOnInspector();
	
	void SetUpMesh();

	void BeginDraw();
	void EndDraw();

	Texture* SetCheckersTexture();

public:
	std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture>  textures;

protected:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Texture* currentTexture;
	Texture* texture;
	Texture* checkersTexture;

};