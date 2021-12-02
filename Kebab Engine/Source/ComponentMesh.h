#pragma once

#include "Component.h"

#include "KbMesh.h"
#include "Texture.h"

#include "Vertex.h"

class ComponentMaterial;
class KbModel;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* compOwner, const std::string& meshPath = "");

	~ComponentMesh();

	void Enable();
	void Disable();

	void Update();
	void Draw(ComponentMaterial* mat);

	void SetData(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* tex = nullptr/*, std::vector<Texture> textures*/);
	void SetMeshPath(const std::string& path);

	void SetMesh(KbMesh* newMesh);
	inline KbMesh* GetMesh() const { return mesh; }
	void SetModel(std::shared_ptr<KbModel>);

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
	std::shared_ptr<KbModel> model;

	bool isKbGeometry = false;

	bool drawVertexNormals;
	bool drawTriangleNormals;

	float normalsVertexSize;
	float3 normalsVertexColor;
	
	float normalsTriangleSize;
	float3 normalsTriangleColor;
};