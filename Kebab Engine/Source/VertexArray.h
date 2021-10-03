#pragma once

#include "Buffer.h"

#include <vector>
#include <memory>

class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(VertexBuffer& vertexBuf);
	void SetIndexBuffer(IndexBuffer& indexBuf); // Set and not Add because you can only have 1 IndexBuffer

	std::vector<VertexBuffer*>& GetVertexBuffers() { return vertexBuffers; }

private:
	uint32_t vao = 0;

	std::vector<VertexBuffer*> vertexBuffers;
	IndexBuffer* indexBuffer;
};