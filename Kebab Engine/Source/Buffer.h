#pragma once

#include "GL/glew.h"

class VertexBuffer
{
public:
	VertexBuffer(/*float* vertex, unsigned int size*/); // TODO: Change size to uint32_t
	virtual ~VertexBuffer();

	//VertexBuffer* Create(float* vertex, unsigned int size); // TODO: Change size to uint32_t

	void SetData(float* vertices, unsigned int size);
	void Bind() const;
	void UnBind() const;

private:
	GLuint vbo;
};

class IndexBuffer
{
public:
	IndexBuffer(/*float* indices, unsigned int size*/); // TODO: Change size to uint32_t
	virtual ~IndexBuffer();
	
	//IndexBuffer* Create(float* indices, unsigned int size); // TODO: Change size to uint32_t
	void SetData(float* indices, unsigned int count);
	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return count; }

private:
	GLuint ibo;
	unsigned int count;
};