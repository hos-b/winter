#include "tests/lighting.h"
#include "framework/base/texture.h"
#include "framework/base/renderer.h"
#include "framework/misc/helper.h"
#include "framework/misc/camera.h"
#include "framework/misc/debug.h"
#include "framework/misc/light.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace winter
{
namespace test
{

LightTest::~LightTest()
{
    glDisable(GL_DEPTH_TEST);
    window()->RemoveInputSubscriber("lit_pyramid");
    delete mesh_;
}
LightTest::LightTest()
{
    // blending options
    GLDebug(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLDebug(glEnable(GL_BLEND));

	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	float vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floor_indices[] = {
		0, 2, 1,
		1, 2, 3
	};

	float floor_vertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	helper::CalculatePhongNormals(indices, 12, vertices, 32, 8, 5);

	// mesh
    mesh_ = new mesh::Mesh("pyramid");
    mesh_->AddVertexBufferElement<float>(3);
    mesh_->AddVertexBufferElement<float>(2);
	mesh_->AddVertexBufferElement<float>(3);
    mesh_->CreateMesh(vertices, 4 * 8 * sizeof(float), indices, 12);

	floor_ = new mesh::Mesh("floor");
	floor_->AddVertexBufferElement<float>(3);
    floor_->AddVertexBufferElement<float>(2);
	floor_->AddVertexBufferElement<float>(3);
	floor_->CreateMesh(floor_vertices, 4 * 8 * sizeof(float), floor_indices, 6);

	// shaders
    shader_ = new base::Shader("../res/shaders/advanced/lighting_vertex.glsl", "../res/shaders/advanced/lighting_frag.glsl");
    mesh_->AssignShader(shader_);
	floor_->AssignShader(shader_);
	shader_->Bind();

	// texture
    texture_ = new base::Texture("../res/textures/brick.png", GL_REPEAT);
	texture_->Bind(0);
	dirt_ = new base::Texture("../res/textures/plain.png", GL_REPEAT);
	dirt_->Bind(0);
	shader_->SetUniform<int, 1>("u_texture", 0);

	// lighting : directional
    directional_light_ = new util::DirectionalLight("directional_light", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f);
    directional_light_->UpdateUniforms(shader_);
	//lighting : point lights
	point_lights_ = new util::PointLight*[3];
	point_lights_[0] = new util::PointLight("light0", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f), 0.0f, 0.4f);
	point_lights_[0]->SetFadingParameters(0.3f, 0.2f, 0.1f);
	point_lights_[0]->UpdateUniforms(shader_);
	point_lights_[1] = new util::PointLight("light1", glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(4.0f, 0.0f, 0.0f), 0.0f, 1.0f);
	point_lights_[1]->SetFadingParameters(0.3f, 0.2f, 0.1f);
	point_lights_[1]->UpdateUniforms(shader_);
	//point_lights_[2] = new util::PointLight("light2", glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 4.0f, 0.0f), 0.4f, 0.1f);
	//point_lights_[2]->SetFadingParameters(0.6f, 0.5f, 0.16f);
	//point_lights_[2]->UpdateUniforms(shader_);

	// material
	material_ = new mesh::Material("material", 4.0f, 256.0f);
	material_->UpdateUnifrorms(shader_);

	// camera
    camera_ = new util::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.0f);
    camera_->SetSensitivity(5.0f, 20.0f);

    // projection, view, model matrices
    projection_ = glm::perspective(45.0f, 1366.0f/768.0f, 0.1f, 500.0f);
    shader_->SetUniform<float*, 16>("u_projection", &projection_[0][0]);

    // enable depth buffer
    glEnable(GL_DEPTH_TEST);
}
void LightTest::SetWindowReference(util::Window* window)
{
    Test::SetWindowReference(window);
    window->RegsiterInputSubscriber(camera_, "lit_pyramid");
}
void LightTest::OnRender()
{
	base::Renderer::Clear(base::Renderer::RenderMode::GL3D);
    // renderer binds both buffers before the draw call
    shader_->Bind();
    shader_->SetUniform<float*, 16>("u_view", &camera_->view_matrix()[0][0]);
    shader_->SetUniform<float*, 16>("u_projection", &projection_[0][0]);
	shader_->SetUniform<glm::vec3, 3>("u_camera", camera_->position());

	texture_->Bind(0);
	model_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	mesh_->OnRender();

	dirt_->Bind(0);
	model_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	floor_->OnRender();
}
void LightTest::OnImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &translation_.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Rotation B", &rotation_.x, 0.0f, 2*glm::pi<float>());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

} // namespace test
} // namespace winter