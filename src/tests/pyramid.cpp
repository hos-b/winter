#include "tests/pyramid.h"
#include "framework/base/renderer.h"
#include "framework/util/debug.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace winter
{
namespace test
{

PyramidTest::~PyramidTest()
{
    glDisable(GL_DEPTH_TEST);
    delete va_;
    delete ib_;
    delete vb_;
}
PyramidTest::PyramidTest()
{
    // blending options
    GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLDebug(glEnable(GL_BLEND));

    float positions[] = {
       -100.0f  , -100.0f   , 0.0f  ,  // 0
        0.0f    ,  0.0f     , 100.0f,  // 1
        100.0f  , -100.0f   , 0.0f  ,  // 2
        0.000f  ,  100.0f   , 0.0f     // 3
    };
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    // vertex array
    va_ = new base::VertexArray;
    // vertex buffer
    vb_ = new base::VertexBuffer(positions, 4 * 3 *sizeof(float));
    // layouts
    base::VertexBufferLayout layout;
    layout.Push<float>(3);
    va_->AddBuffer(*vb_, layout);

    // index buffer
    ib_ = new base::IndexBuffer(indices, 12);

    // shaders
    shader_ = new base::Shader("../res/shaders/basic_shader_notex.glsl");
    shader_->Bind();

    // projection, view, model matrces
    // projection_ = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -400.0f, 400.0f);
    projection_ = glm::perspective(45.0f, 640.0f/480.0f, 0.1f, 500.0f);
    view_ =  glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -300.0f));
    model_ = glm::translate(glm::mat4(1.0f), glm::vec3(150, 150, 0));
    mvp_ = projection_ * view_ * model_;
    shader_->SetUniform<glm::mat4, 16>("u_model_view_projection", mvp_);

    // enable depth buffer
    glEnable(GL_DEPTH_TEST);
}
void PyramidTest::OnRender()
{
    base::Renderer::Clear(base::Renderer::RenderMode::GL3D);
    // renderer binds both buffers before the draw call
    shader_->Bind();
    model_ = glm::translate(glm::mat4(1.0f), translation_);
    model_ = glm::rotate(model_, rotation_.x ,glm::vec3(1,0,0));
    model_ = glm::rotate(model_, rotation_.y ,glm::vec3(0,1,0));
    model_ = glm::rotate(model_, rotation_.z ,glm::vec3(0,0,1));
    mvp_ = projection_ * view_ * model_;
    shader_->SetUniform<float*, 16>("u_model_view_projection", &mvp_[0][0]);
    base::Renderer::Draw(*va_, *ib_, *shader_);
    rotation_.y += 0.01f;
    if (rotation_.y > 2*glm::pi<float>())
        rotation_.y-=2*glm::pi<float>();
}
void PyramidTest::OnImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &translation_.x, -200.0f, 200.0f);
    ImGui::SliderFloat3("Rotation B", &rotation_.x, 0.0f, 2*glm::pi<float>());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

}// end of test
}// end of winter