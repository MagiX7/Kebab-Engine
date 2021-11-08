#pragma once

#include "Component.h"

#include "Mesh.h"
#include "Texture.h"

#include "Vertex.h"

class ComponentMaterial;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject& compOwner, const std::string& meshPath = "");
	~ComponentMesh();

	void Enable();
	void Disable();

	void Update();
	void Draw(ComponentMaterial* mat);

	void SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* tex = nullptr/*, std::vector<Texture> textures*/);
	void SetMeshPath(const std::string& path);

	inline KbMesh* GetMesh() const { return mesh; }

	JSON_Value* Save() override;
	void Load(JSON_Object* obj, GameObject* parent = nullptr) override;

private:
	void DrawOnInspector();

	void BeginDraw(ComponentMaterial* mat);
	void EndDraw(ComponentMaterial* mat);

public:
	/*std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture>  textures;*/

protected:
	/*VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	std::string meshPath;
	std::string meshName;*/

	KbMesh* mesh;

	bool isKbGeometry = false;

	bool drawVertexNormals;
	bool drawTriangleNormals;

	float normalsVertexSize;
	float3 normalsVertexColor;
	
	float normalsTriangleSize;
	float3 normalsTriangleColor;
};