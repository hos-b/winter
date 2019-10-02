#include "index_buffer.h"
#include "renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : count_(count)
{
    GLDebug(glGenBuffers(1, &renderer_id_));
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
    GLDebug(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer()
{
    GLDebug(glDeleteBuffers(1, &renderer_id_));
}

void IndexBuffer::Bind() const
{
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
}
void IndexBuffer::Unbind() const
{
    GLDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
unsigned int IndexBuffer::count() const
{
    return count_;
}