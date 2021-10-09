#include "Buffer.h"

#include "mmgr/mmgr.h"

////////////////////////////////////////////////////////
// VertexBuffer ////////////////////////////////////////
////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer()
{
	layout = { {ShaderDataType::VEC3F, "position"} };
	glGenBuffers(1, &vbo);
}

VertexBuffer::VertexBuffer(const float3* vertices, uint32_t size)
{
	layout = { {ShaderDataType::VEC3F, "position"} };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vbo);
}

void VertexBuffer::SetData(const float3* vertices, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

////////////////////////////////////////////////////////
// IndexBuffer /////////////////////////////////////////
////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer() : count(0)
{
	glGenBuffers(1, &ibo);
}

IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count) : count(count)
{
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &ibo);
}

void IndexBuffer::SetData(const uint32_t* indices, uint32_t count)
{
	this->count += count;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}