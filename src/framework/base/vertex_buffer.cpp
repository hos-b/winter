#include "framework/base/vertex_buffer.h"
#include "framework/util/debug.h"
#include <GL/glew.h>

namespace winter
{
namespace base
{

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : count_(size)
{
    GLDebug(glGenBuffers(1, &renderer_id_));
    GLDebug(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
    GLDebug(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer()
{
    GLDebug(glDeleteBuffers(1, &renderer_id_));
}

void VertexBuffer::Bind() const
{
    GLDebug(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
}
void VertexBuffer::Unbind() const
{
    GLDebug(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

} // namespace base
} // namespace winter