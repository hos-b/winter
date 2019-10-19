#ifndef __TEST_TEXTURE_H__
#define __TEST_TEXTURE_H__

#include <glm/glm.hpp>
#include "test.h"
#include "framework/base/shader.h"
#include "framework/base/texture.h"
#include "framework/base/vertex_array.h"
#include "framework/base/index_buffer.h"
#include "framework/base/vertex_buffer.h"


namespace winter
{
namespace test
{

class TextureTest : public Test
{
public:
    TextureTest();
    ~TextureTest();
    void OnRender();
    void OnImGuiRender();
private:
    glm::mat4 model_, view_, projection_, mvp_;
    glm::vec3 translation_a_, translation_b_;
    glm::vec3 rotation_;

    base::Shader* shader_;
    base::IndexBuffer* ib_;
    base::VertexArray* va_;
    base::VertexBuffer* vb_;
    base::Texture* texture_;
};

} // namespace test
} // namespace winter

#endif