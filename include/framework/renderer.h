#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "framework/index_buffer.h"
#include "framework/vertex_array.h"
#include "framework/shader.h"

class Renderer
{
public:
    void Clear() const;
    void SetClearColor() const;
    void Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const;
private:
};
#endif