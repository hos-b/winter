#include "framework/renderer.h"
#include "framework/debug.h"

void Renderer::Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const
{
    shader.Bind();
    vertex_array.Bind();
    index_buffer.Bind();
    GLDebug(glDrawElements(GL_TRIANGLES, index_buffer.count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLDebug(glClear(GL_COLOR_BUFFER_BIT));
}