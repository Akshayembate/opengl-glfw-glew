#include "vertexBuffer.h"
#include "renderer.h"

vertexBuffer::vertexBuffer(const void* data, unsigned int size)
{
   GLCall(glGenBuffers(1, &m_RendererID));
   GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
   GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); 
}

vertexBuffer::~vertexBuffer()
{
}

void vertexBuffer::bind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void vertexBuffer::unBind()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}