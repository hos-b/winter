#include "framework/base/renderer.h"
#include "framework/misc/debug.h"

#include <iostream>

namespace winter
{
namespace base
{

Renderer::Renderer(RenderMode mode): render_mode_(mode){}

void Renderer::Clear() const
{
    if (render_mode_ == RenderMode::GL2D)
    {
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));
        
    }
    else if (render_mode_ == RenderMode::GL3D)
    {
        GLDebug(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
    }
}

// static functions
void Renderer::Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader)
{
    shader.Bind();
    vertex_array.Bind();
    index_buffer.Bind();
    GLDebug(glDrawElements(GL_TRIANGLES, index_buffer.count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(Renderable& object)
{
    object.OnRender();
}

void Renderer::Clear(RenderMode mode)
{
    if (mode == RenderMode::GL2D)
    {
        GLDebug(glClear(GL_COLOR_BUFFER_BIT));
    }
    else if (mode == RenderMode::GL3D)
    {
        GLDebug(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
    }
}

} // namespace base
} // namespace winter