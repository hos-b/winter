#ifndef __PYRAMID_TEST_H__
#define __PYRAMID_TEST_H__

#include <glm/glm.hpp>

#include "test.h"
#include "framework/mesh/mesh.h"
#include "framework/base/shader.h"
#include "framework/base/glf_texture.h"
#include "framework/util/camera.h"

namespace winter
{
namespace test
{
    
class PyramidTest : public Test
{
public:
    PyramidTest();
    ~PyramidTest();
    void OnRender();
    void OnImGuiRender();
    void SetWindowReference(util::Window* window) override;
private:
    glm::mat4 model_, view_, projection_, mvp_;
    glm::vec3 translation_;
    glm::vec3 rotation_;

    base::Shader* shader_;
    base::Texture *texture_;
    util::Camera *camera_;
    mesh::Mesh* mesh_;
    ;
};

} // namespace test
} // namespace winter

#endif