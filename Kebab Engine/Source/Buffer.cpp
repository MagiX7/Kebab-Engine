#include "Buffer.h"

#include "Globals.h"

#include "mmgr/mmgr.h"

////////////////////////////////////////////////////////
// VertexBuffer ////////////////////////////////////////
////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer() : count(0)
{
	layout = { {ShaderDataType::VEC3F, "position"} };
	glGenBuffers(1, &vbo);
}

VertexBuffer::VertexBuffer(const float3* vertices, uint32_t size) : count(0)
{
	layout = { {ShaderDataType::VEC3F, "position"} };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	count = size / sizeof(uint32_t);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &vbo);
}

void VertexBuffer::SetData(const float3* vertices, uint32_t size)
{
	/*this->vertices.resize(count + (size / sizeof(uint32_t)));
	this->vertices.insert(this->vertices.begin() + this->vertices.size(), vertices, vertices + count);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	count += (size / sizeof(uint32_t));
}

void VertexBuffer::SetData(const float2* vertices, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	count += (size / sizeof(uint32_t));
}

void VertexBuffer::SetData(const std::vector<Vertex>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	count += vertices.size();
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &ibo);
}

void IndexBuffer::SetData(const uint32_t* indices, uint32_t count)
{
	/*this->indices.resize(this->count + count);
	this->indices.insert(this->indices.begin() + this->indices.size(), indices, indices + this->count);
	*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, count, count * sizeof(uint32_t), indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	this->count += count;
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

FrameBuffer::FrameBuffer(const FrameBufferProperties& props) : properties(props)
{
	Create();


	/*glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/


	//glGenFramebuffers(1, &id);

	//glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);

	//// TODO: Should use glTexStorage? Very useful for 3d graphics shader
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, properties.width, properties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &colorAttachment);
	glDeleteTextures(1, &depthAttachment);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, properties.width, properties.height);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Create()
{
	if (fbo)
	{
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &colorAttachment);
		glDeleteTextures(1, &depthAttachment);
	}

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &colorAttachment);
	glBindTexture(GL_TEXTURE_2D, colorAttachment);
	
	// TODO: Should use glTexStorage? Very useful for 3d graphics shader
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, properties.width, properties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &depthAttachment);
	glBindTexture(GL_TEXTURE_2D, depthAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, properties.width, properties.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, properties.width, properties.height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_CONSOLE("Framebuffer is Incomplete. Error %s", glGetString(err));
	}
	else LOG_CONSOLE("Framebuffer is Complete");

	glBindFramebuffer(GL_TEXTURE_2D, 0);
}

void FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	properties.width = width;
	properties.height = height;
	Create();
}