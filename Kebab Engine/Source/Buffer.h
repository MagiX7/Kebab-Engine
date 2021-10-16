#pragma once

//#include "Mesh.h"

#include "Vertex.h"

#include "Math/float3.h"
#include "Math/float2.h"

#include "GL/glew.h"

#include <stdint.h>
#include <vector>
#include <string>

enum class ShaderDataType
{
	NONE = 0,
	FLOAT,
	VEC2F,
	VEC3F,
	VEC4F,
	MAT3,
	MAT4,
	INT,
	VEC2I,
	VEC3I,
	VEC4I,
	BOOL
};

static uint32_t GetShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::FLOAT:  return 4;
		case ShaderDataType::VEC2F:  return 4 * 2;
		case ShaderDataType::VEC3F:  return 4 * 3;
		case ShaderDataType::VEC4F:  return 4 * 4;
		case ShaderDataType::MAT3:	 return 4 * 3 * 3;
		case ShaderDataType::MAT4:	 return 4 * 4 * 4;
		case ShaderDataType::INT:	 return 4;
		case ShaderDataType::VEC2I:	 return 4 * 2;
		case ShaderDataType::VEC3I:	 return 4 * 3;
		case ShaderDataType::VEC4I:	 return 4 * 4;
		case ShaderDataType::BOOL:	 return 1;
	}
	return 0;
}

static GLenum GetOpenGLRawTypeFromShaderDataType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::FLOAT:  return GL_FLOAT;
	case ShaderDataType::VEC2F:  return GL_FLOAT;
	case ShaderDataType::VEC3F:  return GL_FLOAT;
	case ShaderDataType::VEC4F:  return GL_FLOAT;
	case ShaderDataType::MAT3:	 return GL_FLOAT;
	case ShaderDataType::MAT4:	 return GL_FLOAT;
	case ShaderDataType::INT:	 return GL_INT;
	case ShaderDataType::VEC2I:	 return GL_INT;
	case ShaderDataType::VEC4I:	 return GL_INT;
	case ShaderDataType::VEC3I:	 return GL_INT;
	case ShaderDataType::BOOL:	 return GL_BOOL;
	}
	return 0;
}

struct BufferElement
{
	std::string name;
	ShaderDataType type;
	uint32_t size;
	uint32_t offset;
	bool normalized;

	BufferElement() {}
	BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : name(name), type(type), size(GetShaderDataTypeSize(type)), offset(0), normalized(normalized) {}

	uint32_t GetComponentCount() const
	{
		switch (type)
		{
		case ShaderDataType::FLOAT:	 return 1;
		case ShaderDataType::VEC2F:  return 2;
		case ShaderDataType::VEC3F:  return 3;
		case ShaderDataType::VEC4F:  return 4;
		case ShaderDataType::MAT3:   return 3 * 3;
		case ShaderDataType::MAT4:   return 4 * 4;
		case ShaderDataType::INT:	 return 1;
		case ShaderDataType::VEC2I:  return 2;
		case ShaderDataType::VEC3I:  return 3;
		case ShaderDataType::VEC4I:  return 4;
		case ShaderDataType::BOOL:   return 1;
		}
		return 0;
	}

};

class BufferLayout
{
public:
	BufferLayout() {}
	BufferLayout(const std::initializer_list<BufferElement>& elements) : bufferElements(elements)
	{
		// Offset and stride calculation
		stride = 0; 
		uint32_t offset = 0;
		for (auto& el : bufferElements)
		{
			el.offset = offset;
			offset += el.size;
			stride += el.size;
		}
	}

	inline const std::vector<BufferElement>& GetElements() const { return bufferElements; }
	inline uint32_t GetStride() const { return stride; }

private:
	std::vector<BufferElement> bufferElements;

	uint32_t stride = 0; // Distance to the next vertex
};

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const float3* vertices, uint32_t size);
	virtual ~VertexBuffer();

	void SetData(const float3* vertices, uint32_t size);
	void SetData(const float2* vertices, uint32_t size);
	void SetData(const std::vector<Vertex>&);
	void Bind() const;
	void Unbind() const;

	inline const BufferLayout& GetLayout() const { return layout; }
	// Make sure to do this BEFORE adding the VertexBuffer into the VertexArray.
	inline void SetLayout(const BufferLayout& l) { layout = l; }

private:
	GLuint vbo;
	uint32_t count;
	BufferLayout layout;

	//std::vector<float3> vertices;
};

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(const uint32_t* indices, uint32_t count);
	virtual ~IndexBuffer();
	
	void SetData(const uint32_t* indices, uint32_t count);
	void Bind() const;
	void Unbind() const;

	inline const uint32_t &GetCount() const { return count; }
	inline const GLuint &GetID() const { return ibo; }


private:
	GLuint ibo;
	uint32_t count;
	//std::vector<uint32_t> indices;
};


struct FrameBufferProperties
{
	unsigned int width, height;
	unsigned int samples = 1;

	// In case you want to draw to the screen, set this to true
	bool swapChainTarget = false;
};

class FrameBuffer
{
public:
	FrameBuffer(const FrameBufferProperties& props);
	virtual ~FrameBuffer();

	void Create();
	void Resize(unsigned int width, unsigned int height);
	
	void Bind() const;
	void Unbind() const;

	inline const FrameBufferProperties& GetProperties() const { return properties; }
	inline const uint32_t GetColorAttachment() const { return colorAttachment; }


private:
	uint32_t fbo = 0;
	uint32_t colorAttachment = 0;
	uint32_t depthAttachment = 0;
	FrameBufferProperties properties;
};