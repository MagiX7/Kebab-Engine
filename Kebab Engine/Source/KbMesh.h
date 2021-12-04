#pragma once

#include "Resource.h"

#include "Buffer.h"
#include "Vertex.h"

#include <vector>

class Texture;

class KbMesh : public Resource
{
public:
	KbMesh() : Resource(ResourceType::MESH) {};
	KbMesh(std::string path);
	KbMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	virtual ~KbMesh();

	void SetData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	void BeginDraw();
	void EndDraw();

	void DrawVertexNormals(const float& size, const float3& color);
	void DrawTriangleNormals(const float& size, const float3& color);

	inline const std::vector<Vertex>& GetVertices() const { return vertices; }
	inline const std::vector<uint32_t>& GetIndices() const { return indices; }
	inline const IndexBuffer* GetIndexBuffer() const { return indexBuffer; }
	inline const std::string& GetPath() const { return path; }
	inline const std::string& GetName() const { return name; }
	inline const std::string& GetOwnerName() const { return ownerName; }
	inline const std::string& GetTextureMetaPath() const { return textureMetaPath; }

	void SetPath(const std::string&);
	void SetName(const std::string&);
	void SetOwnerName(const std::string&);
	inline void SetTextureMetaPath(const std::string& path) { textureMetaPath = path; }

protected:
	void SetUpMesh();

public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	/*std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;*/
	std::string textureMetaPath;

	std::string path;
	std::string name;
	std::string ownerName;
};