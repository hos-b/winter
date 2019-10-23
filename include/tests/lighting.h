#ifndef __LIGHTING_TEST_H__
#define __LIGHTING_TEST_H__

#include <glm/glm.hpp>

#include "test.h"
#include "framework/mesh/material.h"
#include "framework/mesh/mesh.h"
#include "framework/mesh/model.h"
#include "framework/base/texture.h"
#include "framework/base/shader.h"
#include "framework/base/shadow_map.h"
#include "framework/misc/camera.h"
#include "framework/misc/light.h"

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
	void render_shadowmap();

	glm::mat4 model_, view_, projection_;
    glm::vec3 translation_;
    glm::vec3 rotation_;

	base::Shader *shader_, *shadow_shader_;
	base::Texture *texture_, *dirt_;
	util::Camera *camera_;
	util::DirectionalLight *directional_light_;
	util::PointLight **point_lights_;
	util::SpotLight **spot_lights_;
	mesh::Mesh *mesh_, *floor_;
	mesh::Model *shelby_;
	mesh::Material *material_;
};

} // namespace test
} // namespace winter

#endif