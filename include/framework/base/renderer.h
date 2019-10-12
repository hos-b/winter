#ifndef __WINTER_RENDERER_H__
#define __WINTER_RENDERER_H__

#include "framework/base/abstract_types.h"
#include "framework/base/index_buffer.h"
#include "framework/base/vertex_array.h"
#include "framework/base/shader.h"

namespace winter
{
namespace base
{

class Renderer
{
public:
    enum class RenderMode{GL2D, GL3D};
    Renderer(RenderMode mode = RenderMode::GL2D);
    void Clear() const;
    void SetClearColor() const;

    // static functions
    static void Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader);
    static void Draw(Renderable& object);
    static void Clear(RenderMode mode);
private:
    RenderMode render_mode_;

};

} // namespace base
} // namespace winter
#endif