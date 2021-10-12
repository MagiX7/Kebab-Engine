#pragma once

#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "Geometry.h"

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
	
	void SetDepth(bool value);
	void SetCullFace(bool value);
	void SetLighting(bool value);
	void SetColorMaterial(bool value);
	void SetTexture2D(bool value);
	void SetWireframe(bool value);

	void Save(JSON_Object* root) override;
	void Load(JSON_Object* root) override;

	void Submit(KebabGeometry* geometry);
	void Submit(std::vector<KebabGeometry*> geos);

	void DoDraw();

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

private:
	std::vector<KebabGeometry*> geometries;
	
	VertexArray* vertexArray;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	std::vector<float3> vertices;
	std::vector<uint32_t> indices;
	uint32_t numIndices;
	uint32_t numVertices;
};