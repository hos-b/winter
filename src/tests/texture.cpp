#include "tests/texture.h"
#include "framework/base/renderer.h"
#include "framework/util/debug.h"
#include <glm/gtc/matrix_transform.hpp>

namespace winter
{
namespace test
{

TextureTest::~TextureTest()
{
    delete va_;
    delete ib_;
    delete vb_;
    delete texture_;
}
TextureTest::TextureTest()
{
    // blending options
    GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLDebug(glEnable(GL_BLEND));

    float positions[] = {
       -100.0f, -100.0f, 0.0f, 0.0f,  // 0
        100.0f, -100.0f, 3.0f, 0.0f,  // 1
        100.0f,  100.0f, 3.0f, 3.0f,  // 2
       -100.0f,  100.0f, 0.0f, 3.0f   // 3
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // vertex array
    va_ = new base::VertexArray;
    // vertex buffer
    vb_ = new base::VertexBuffer(positions, 4 * 4 *sizeof(float));
    // layouts
    base::VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va_->AddBuffer(*vb_, layout);

    // index buffer
    ib_ = new base::IndexBuffer(indices, 6);

    // shaders
    shader_ = new base::Shader("../res/shaders/basic_shader.glsl");
    shader_->Bind();

    // projection, view, model matrces
    projection_ = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    view_ =  glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    model_ = glm::translate(glm::mat4(1.0f), glm::vec3(150, 150, 0));
    mvp_ = projection_ * view_ * model_;
    shader_->SetUniform<glm::mat4, 16>("u_model_view_projection", mvp_);
    // textures
    texture_ = new base::Texture("../res/textures/fb.png", GL_REPEAT);
    texture_->Bind();
    shader_->SetUniform<int, 1>("u_texture", 0);
}
void TextureTest::OnRender()
{
    // draw the current bound index buffer of type uint, count 6
    // renderer binds both buffer before the draw call
    shader_->Bind();
    model_ = glm::translate(glm::mat4(1.0f), translation_a_);
    mvp_ = projection_ * view_ * model_;
    shader_->SetUniform<float*, 16>("u_model_view_projection", &mvp_[0][0]);
    base::Renderer::Draw(*va_, *ib_, *shader_);

    model_ = glm::translate(glm::mat4(1.0f), translation_b_);
    model_ = glm::rotate(model_, rotation_.x ,glm::vec3(1,0,0));
    model_ = glm::rotate(model_, rotation_.y ,glm::vec3(0,1,0));
    model_ = glm::rotate(model_, rotation_.z ,glm::vec3(0,0,1));
    mvp_ = projection_ * view_ * model_;
    shader_->SetUniform<float*, 16>("u_model_view_projection", &mvp_[0][0]);
    base::Renderer::Draw(*va_, *ib_, *shader_);

    shader_->SetUniform<float, 1>("float1", 1.0f);
    shader_->SetUniform<float, 4>("float4", 1.0f, 2.0f, 1.1f, 2.2f);
    shader_->SetUniform<int, 1>("int1", 1);
}
void TextureTest::OnImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &translation_a_.x, 0.0f, 640.0f);
    ImGui::SliderFloat3("Translation B", &translation_b_.x, 0.0f, 640.0f);
    ImGui::SliderFloat3("Rotation B", &rotation_.x, 0.0f, 2*glm::pi<float>());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

}// end of test
}// end of winter