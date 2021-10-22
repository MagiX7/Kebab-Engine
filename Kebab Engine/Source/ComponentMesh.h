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
	void Draw(bool showNormals = false);

	void SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

private:
	void SetUpMesh();

	void BeginDraw();
	void EndDraw();

	void SetCheckersTexture();

public:
	std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture>  textures;

protected:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	Texture* texture;

};