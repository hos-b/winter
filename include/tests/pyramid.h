#ifndef __PYRAMID_TEST_H__
#define __PYRAMID_TEST_H__

#include <glm/glm.hpp>

#include "test.h"
#include "framework/mesh/mesh.h"
#include "framework/base/shader.h"
#include "framework/base/glf_texture.h"


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
private:
    glm::mat4 model_, view_, projection_, mvp_;
    glm::vec3 translation_;
    glm::vec3 rotation_;

    base::Shader* shader_;
    mesh::Mesh* mesh_;
    ;
};

} // namespace test
} // namespace winter

#endif