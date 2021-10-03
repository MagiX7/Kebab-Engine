#include "VertexArray.h"

#include "mmgr/mmgr.h"

VertexArray::VertexArray()
{
    // We do not bind it here because if we later create a vertex array and its bound, they are linked together and that may be dangerous.
	glCreateVertexArrays(1, &vao);

    //indexBuffer = new IndexBuffer();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao);
    for (auto& v : vertexBuffers)
    {
        v->~VertexBuffer();
        v++;
    }
    vertexBuffers.clear();
    indexBuffer->~IndexBuffer();
}

void VertexArray::Bind() const
{
	glBindVertexArray(vao);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

// This should be called AFTER setting the layout to the VertexBuffer.
void VertexArray::AddVertexBuffer(VertexBuffer& vertexBuf)
{
    if (vertexBuf.GetLayout().GetElements().size() > 0)
    {
        glBindVertexArray(vao);
        vertexBuf.Bind();

        uint32_t index = 0;
        const auto& layout = vertexBuf.GetLayout();
        for (const auto& element : layout.GetElements())
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                element.GetComponentCount(),
                GetOpenGLRawTypeFromShaderDataType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.offset);
            index++;
        }
        vertexBuffers.push_back(&vertexBuf);
    }
}

void VertexArray::SetIndexBuffer(IndexBuffer& indexBuf)
{
    glBindVertexArray(vao);
    indexBuf.Bind();
    indexBuffer = &indexBuf;

}