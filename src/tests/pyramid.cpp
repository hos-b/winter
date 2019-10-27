#include "tests/pyramid.h"
#include "framework/base/renderer.h"
#include "framework/misc/camera.h"
#include "framework/misc/debug.h"
#include "framework/base/texture.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace winter
{
namespace test
{

PyramidTest::~PyramidTest()
{
    glDisable(GL_DEPTH_TEST);
    window()->RemoveInputSubscriber("pyramid");
    delete mesh_;
}
PyramidTest::PyramidTest()
{
    // blending options
    GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLDebug(glEnable(GL_BLEND));
    
    float positions[] = {
       -100.0f  , -100.0f   , 0.0f  ,   0.0f, 0.0f, // 0
        0.0f    ,  0.0f     , 100.0f,   0.5f, 0.0f, // 1
        100.0f  , -100.0f   , 0.0f  ,   1.0f, 0.0f, // 2
        0.0f    ,  100.0f   , 0.0f  ,   0.5f, 1.0f  // 3
    };
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    // mesh
    mesh_ = new mesh::Mesh("pyramid");
    mesh_->AddVertexBufferElement<float>(3);
    mesh_->AddVertexBufferElement<float>(2);
    mesh_->CreateMesh(positions, 4 * 5 * sizeof(float), indices, 12);

    // shaders
    shader_ = new base::Shader("../res/shaders/basic/basic_shader.glsl");
    shader_->Bind();

    // texture
    texture_ = new base::Texture("../res/textures/opengl.png", GL_REPEAT);
    texture_->Bind(0);
    shader_->SetUniform<int, 1>("u_texture", 0);

    // camera
    camera_ = new util::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.0f);
    camera_->SetSensitivity(200.0f, 20.0f);


    // projection, view, model matrices
    // projection_ = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -400.0f, 400.0f);
    projection_ = glm::perspective(45.0f, 640.0f/480.0f, 0.1f, 500.0f);
    view_ =  glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -300.0f));
    model_ = glm::translate(glm::mat4(1.0f), glm::vec3(150, 150, 0));
    mvp_ = projection_ * view_ * model_;
    shader_->SetUniform<glm::mat4, 16>("u_model_view_projection", mvp_);

    // enable depth buffer
    glEnable(GL_DEPTH_TEST);
}
void PyramidTest::SetWindowReference(util::Window* window)
{
    Test::SetWindowReference(window);
    window->RegsiterInputSubscriber(camera_, "pyramid");
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
    mvp_ = projection_ * camera_->view_matrix() * model_;
    shader_->SetUniform<float*, 16>("u_model_view_projection", &mvp_[0][0]);
    mesh_->OnRender();
    rotation_.y += 0.01f;
    if (rotation_.y > 2*glm::pi<float>())
        rotation_.y -= 2*glm::pi<float>();
     if (rotation_.y <= 0)
        rotation_.y = 0;
}
void PyramidTest::OnImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &translation_.x, -200.0f, 200.0f);
    ImGui::SliderFloat3("Rotation B", &rotation_.x, 0.0f, 2*glm::pi<float>());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

} // namespace test
} // namespace winter