#include "tests/lighting.h"
#include "framework/base/texture.h"
#include "framework/base/renderer.h"
#include "framework/misc/helper.h"
#include "framework/misc/camera.h"
#include "framework/misc/debug.h"
#include "framework/light/light.h"
#include "framework/mesh/model.h"

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
	delete floor_;
	delete texture_;
	delete dirt_;
	delete material_;
	delete camera_;
	delete shader_;
	delete directional_light_;
	delete point_lights_[0];
	delete point_lights_[1];
	delete spot_lights_[0];
	delete[] point_lights_;
	delete[] spot_lights_;
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

	// model
	shelby_ = new mesh::Model("../res/models/merc");
	shelby_->LoadModel("../res/models/merc/sls_amg.obj");

	// shaders
	shadow_shader_ = new base::Shader("../res/shaders/advanced/shadow/directional_vertex.glsl");
    shader_ = new base::Shader("../res/shaders/advanced/standard/lighting_vertex.glsl", 
							   "../res/shaders/advanced/standard/lighting_frag.glsl");
	omni_shadow_shader_ = new base::Shader("../res/shaders/advanced/shadow/omni_vertex.glsl",
										   "../res/shaders/advanced/shadow/omni_frag.glsl",
										   "../res/shaders/advanced/shadow/omni_geometry.glsl");
	shader_->Bind();

	// texture
	texture_ = new base::Texture("../res/textures/brick.png", GL_REPEAT);
	texture_->Bind(0);
	dirt_ = new base::Texture("../res/textures/dirt.png", GL_REPEAT);
	dirt_->Bind(0);
	shader_->SetUniform<int, 1>("u_texture", 0);

	// lighting : directional
    directional_light_ = new light::DirectionalLight("directional_light", glm::vec3(1.0f, 0.53f, 0.3f), glm::vec3(-10.0f, -12.0f, 18.5f), 0.1f, 0.4f);
    directional_light_->UpdateUniforms(shader_);
	directional_light_->InitShadowMap(2048, 2048, 20.0f, 20.0f, 200.0f);
	shader_->SetUniform<int, 1>("u_pcf_radius", 2);
	// lighting : point lights
	point_lights_ = new light::PointLight*[2];
	point_lights_[0] = new light::PointLight("light0", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 4.0f), 0.0f, 0.3f);
	point_lights_[0]->SetFadeParameters(0.3f, 0.01f, 0.01f);
	point_lights_[0]->UpdateUniforms(shader_);
	point_lights_[0]->InitShadowMap(1024, 1024, 0.01f, 100.0f);
	point_lights_[1] = new light::PointLight("light1", glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -4.0f), 0.0f, 0.3f);
	point_lights_[1]->SetFadeParameters(0.3f, 0.01f, 0.01f);
	point_lights_[1]->UpdateUniforms(shader_);
	point_lights_[1]->InitShadowMap(1024, 1024, 0.01f, 100.0f);
	// lighting : spot lights
	spot_lights_ = new light::SpotLight*[2];
	spot_lights_[0] = new light::SpotLight("light2", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(4.0f, 4.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f), 40.0f, 0.2f, 0.2f);
	spot_lights_[0]->SetFadeParameters(1.0f, 0.0f, 0.0f);
	spot_lights_[0]->UpdateUniforms(shader_);
	spot_lights_[0]->InitShadowMap(1024, 1024, 0.01f, 100.0f);

	// material
	material_ = new mesh::Material("material", 4.0f, 256.0f);
	material_->UpdateUnifrorms(shader_);

	// camera
    camera_ = new util::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.0f);
    camera_->SetSensitivity(5.0f, 20.0f);

    // projection, view, model matrices
    projection_ = glm::perspective(glm::radians(60.0f), 1366.0f/768.0f, 0.1f, 500.0f);
    shader_->SetUniform<float*, 16>("u_projection", &projection_[0][0]);

	// skybox
	std::vector<std::string> sky_path;
	sky_path.emplace_back("../res/textures/box/cupertin-lake_rt.tga");
	sky_path.emplace_back("../res/textures/box/cupertin-lake_lf.tga");
	sky_path.emplace_back("../res/textures/box/cupertin-lake_up.tga");
	sky_path.emplace_back("../res/textures/box/cupertin-lake_dn.tga");
	sky_path.emplace_back("../res/textures/box/cupertin-lake_bk.tga");
	sky_path.emplace_back("../res/textures/box/cupertin-lake_ft.tga");
	skybox_ = new mesh::Skybox(sky_path);

	// enable depth bufferratio
	glEnable(GL_DEPTH_TEST);
}
void LightTest::SetWindowReference(util::Window* window)
{
    Test::SetWindowReference(window);
    window->RegsiterInputSubscriber(camera_, "lit_pyramid");
}

void LightTest::render_scene()
{
	// restoring viewport size
	glViewport(0, 0, window()->buffer_width(), window()->buffer_height());
	base::Renderer::Clear(base::Renderer::RenderMode::GL3D);
	skybox_->RenderSkybox(camera_->view_matrix(), projection_);

	shader_->Bind();
    // basic uniforms
	shader_->SetUniform<float*, 16>("u_view", &camera_->view_matrix()[0][0]);
    shader_->SetUniform<float*, 16>("u_projection", &projection_[0][0]);
	shader_->SetUniform<glm::vec3, 3>("u_camera", camera_->position());
	// shadow uniforms
	shader_->SetUniform<float *, 16>("u_dlight_transform", &directional_light_->light_transform()[0][0]);
	directional_light_->shadow_map()->BindMap(2);
	shader_->SetUniform<int, 1>("u_directional_shadow_map", 2);

	point_lights_[0]->shadow_map()->BindMap(3);
	shader_->SetUniform<int, 1>("u_omni_shadow_maps[" + std::to_string(0) + "].shadow_map", 3);
	shader_->SetUniform<float, 1>("u_omni_shadow_maps[" + std::to_string(0) + "].far_plane", point_lights_[0]->far_plane());
	point_lights_[1]->shadow_map()->BindMap(4);
	shader_->SetUniform<int, 1>("u_omni_shadow_maps[" + std::to_string(1) + "].shadow_map", 4);
	shader_->SetUniform<float, 1>("u_omni_shadow_maps[" + std::to_string(1) + "].far_plane", point_lights_[1]->far_plane());

	spot_lights_[0]->shadow_map()->BindMap(5);
	shader_->SetUniform<int, 1>("u_omni_shadow_maps[" + std::to_string(2) + "].shadow_map", 5);
	shader_->SetUniform<float, 1>("u_omni_shadow_maps[" + std::to_string(2) + "].far_plane", spot_lights_[0]->far_plane());
	
	// spot light on the camera
	// spot_lights_[0]->SetPosition(camera_->position() + glm::vec3(0.0f, -0.9f, 0.0f));
	// spot_lights_[0]->SetDirection(camera_->direction());
	// spot_lights_[0]->UpdateUniforms(shader_);

	// model uses texture slot 1
	shader_->SetUniform<int, 1>("u_texture", 1);
	model_ = glm::mat4(1.0f);
	model_ = glm::translate(model_, translation_);
	model_ = glm::rotate(model_, rotation_.x,glm::vec3(1.0f, 0.0f, 0.0f));
	model_ = glm::rotate(model_, rotation_.y,glm::vec3(0.0f, 1.0f, 0.0f));
	model_ = glm::rotate(model_, rotation_.z,glm::vec3(0.0f, 0.0f, 1.0f));
	shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	material_->UpdateUnifrorms(shader_);
	shelby_->OnRender();

	shader_->SetUniform<int, 1>("u_texture", 1);
	dirt_->Bind(1);
	model_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	floor_->OnRender();
}
void LightTest::render_directional_shadowmap()
{
	shadow_shader_->Bind();
	// set viewport to be the same size as our frame buffer
	glViewport(0, 0, directional_light_->shadow_map()->shadow_width(), directional_light_->shadow_map()->shadow_height());
	directional_light_->shadow_map()->BindFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	directional_light_->UpdateShadowUniforms(shadow_shader_);
	// rendering the scene with new shader
	// with no textures or any other uniforms
	model_ = glm::mat4(1.0f);
	model_ = glm::translate(model_, translation_);
	model_ = glm::rotate(model_, rotation_.x,glm::vec3(1.0f, 0.0f, 0.0f));
	model_ = glm::rotate(model_, rotation_.y,glm::vec3(0.0f, 1.0f, 0.0f));
	model_ = glm::rotate(model_, rotation_.z,glm::vec3(0.0f, 0.0f, 1.0f));
	shadow_shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	shelby_->OnRender();

	model_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	shadow_shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	floor_->OnRender();
	directional_light_->shadow_map()->UnbindFramebuffer();
}
void LightTest::render_point_shadowmap(light::PointLight *plight)
{
	omni_shadow_shader_->Bind();
	// set viewport to be the same size as our frame buffer
	glViewport(0, 0, plight->shadow_map()->shadow_width(), plight->shadow_map()->shadow_height());
	plight->shadow_map()->BindFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	plight->UpdateShadowUniforms(omni_shadow_shader_);
	// rendering the scene with new shader
	// with no textures or any other uniforms
	model_ = glm::mat4(1.0f);
	model_ = glm::translate(model_, translation_);
	model_ = glm::rotate(model_, rotation_.x,glm::vec3(1.0f, 0.0f, 0.0f));
	model_ = glm::rotate(model_, rotation_.y,glm::vec3(0.0f, 1.0f, 0.0f));
	model_ = glm::rotate(model_, rotation_.z,glm::vec3(0.0f, 0.0f, 1.0f));
	omni_shadow_shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	shelby_->OnRender();

	model_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	omni_shadow_shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	floor_->OnRender();
	plight->shadow_map()->UnbindFramebuffer();
}
void LightTest::render_spot_shadowmap(light::SpotLight *slight)
{
	omni_shadow_shader_->Bind();
	// set viewport to be the same size as our frame buffer
	glViewport(0, 0, slight->shadow_map()->shadow_width(), slight->shadow_map()->shadow_height());
	slight->shadow_map()->BindFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	// IMPORTANT if you dont set the position below the camera
	// it seems like there is no shadow from point light
	// slight->SetPosition(camera_->position() + glm::vec3(0.0f, -0.9f, 0.0f));
	// slight->SetDirection(camera_->direction());
	// slight->UpdateLightTransforms();
	slight->UpdateShadowUniforms(omni_shadow_shader_);
	// rendering the scene with new shader
	// with no textures or any other uniforms
	model_ = glm::mat4(1.0f);
	model_ = glm::translate(model_, translation_);
	model_ = glm::rotate(model_, rotation_.x,glm::vec3(1.0f, 0.0f, 0.0f));
	model_ = glm::rotate(model_, rotation_.y,glm::vec3(0.0f, 1.0f, 0.0f));
	model_ = glm::rotate(model_, rotation_.z,glm::vec3(0.0f, 0.0f, 1.0f));
	omni_shadow_shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	shelby_->OnRender();

	model_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	omni_shadow_shader_->SetUniform<float*, 16>("u_model", &model_[0][0]);
	floor_->OnRender();
	slight->shadow_map()->UnbindFramebuffer();
}
void LightTest::OnRender()
{
	render_directional_shadowmap();
	render_spot_shadowmap(spot_lights_[0]);
	render_point_shadowmap(point_lights_[0]);
	render_point_shadowmap(point_lights_[1]);
	render_scene();
}
void LightTest::OnImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &translation_.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Rotation B", &rotation_.x, 0.0f, 2*glm::pi<float>());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

} // namespace test
} // namespace winter