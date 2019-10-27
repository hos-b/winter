#ifndef __LIGHTING_TEST_H__
#define __LIGHTING_TEST_H__

#include <glm/glm.hpp>

#include "test.h"
#include "framework/mesh/mesh.h"
#include "framework/mesh/model.h"
#include "framework/mesh/skybox.h"
#include "framework/mesh/material.h"
#include "framework/base/shader.h"
#include "framework/base/texture.h"
#include "framework/misc/camera.h"
#include "framework/light/light.h"
#include "framework/light/shadow_map.h"
#include "framework/light/omni_shadow_map.h"
namespace winter
{
namespace test
{
    
class LightTest : public Test
{
public:
    LightTest();
    ~LightTest();
    void OnRender();
    void OnImGuiRender();
    void SetWindowReference(util::Window* window) override;
private:
	void render_scene();
	void render_directional_shadowmap();
	void render_point_shadowmap(light::PointLight*);
	void render_spot_shadowmap(light::SpotLight*);

	glm::mat4 model_, view_, projection_;
    glm::vec3 translation_;
    glm::vec3 rotation_;

	base::Shader *shader_, *shadow_shader_, *omni_shadow_shader_;
	base::Texture *texture_, *dirt_;
	util::Camera *camera_;
	light::DirectionalLight *directional_light_;
	light::PointLight **point_lights_;
	light::SpotLight **spot_lights_;
	mesh::Mesh *mesh_, *floor_;
	mesh::Model *shelby_;
	mesh::Material *material_;
	mesh::Skybox *skybox_;
};

} // namespace test
} // namespace winter

#endif