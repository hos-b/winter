#ifndef __TEST_TEXTURE_H__
#define __TEST_TEXTURE_H__

#include <glm/glm.hpp>
#include "test.h"
#include "framework/shader.h"
#include "framework/glf_texture.h"
#include "framework/vertex_array.h"
#include "framework/index_buffer.h"
#include "framework/vertex_buffer.h"


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

        Shader* shader_;
        IndexBuffer* ib_;
        VertexArray* va_;
        VertexBuffer* vb_;
        Texture* texture_;
    };
}

#endif