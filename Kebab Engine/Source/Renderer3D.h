#pragma once

#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "Geometry.h"
#include "MeshLoader.h"

#include "ComponentMesh.h"

#include "glmath.h"

#define MAX_LIGHTS 8

class Renderer3D : public Module
{
public:
	Renderer3D(bool startEnabled = true);
	~Renderer3D();

	bool Init(JSON_Object* root);
	bool PreUpdate(float dt);
	bool Draw(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	
	void SetDepth();
	void SetCullFace();
	void SetLighting();
	void SetColorMaterial();
	void SetTexture2D();
	void SetWireframe();

	void Save(JSON_Object* root) override;
	void Load(JSON_Object* root) override;

	void Submit(KbGeometry* geometry);
	void Submit(const std::vector<KbGeometry>& geos);

	void Submit(GameObject* go);

	void DrawGrid();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	//mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;

	bool depth;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
	bool wireframe;

	bool drawVertexNormals;
	bool drawTriangleNormals;

private:
	std::vector<KbGeometry*> geometries;
	std::vector<ComponentMesh*> meshes;

	
	VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	FrameBuffer* frameBuffer;

	std::vector<float3> vertices;
	std::vector<uint32_t> indices;
	uint32_t numIndices;
	uint32_t numVertices;
};