#ifndef __WINTER_LIGHT__
#define __WINTER_LIGHT__

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "framework/base/shader.h"

namespace winter
{
namespace util
{

class Light
{
public:
    Light(const Light &) = delete;
    Light();
    Light(std::string name,glm::vec3 color      , float ambient_intensity,
                           glm::vec3 direction  , float diffuse_intensity);

    void UpdateUniforms(base::Shader *shader);
    void Unbind();

    ~Light();

private:
    std::string name_;
    // ambient lighting
    glm::vec3 color_;
    float ambient_intensity_;
    // diffuse lighting
    glm::vec3 direction_;
    float diffuse_intensity_;
};

} // namespace util
} // namespace winter
#endif