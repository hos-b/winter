#include "framework/misc/light.h"
#include "framework/misc/debug.h"

namespace winter
{
namespace util
{

Light::Light()
{
    color_ = glm::vec3(1.0f, 1.0f, 1.0f);
    ambient_intensity_ = 1.0f;
    direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
    diffuse_intensity_ = 0.0f;
    name_ = "light";
}
Light::Light(std::string name,glm::vec3 color      , float ambient_intensity,
                              glm::vec3 direction  , float diffuse_intensity)
{
    color_ = color;
    ambient_intensity_ = ambient_intensity;
    direction_ = glm::normalize(direction);
    diffuse_intensity_ = diffuse_intensity;
    name_ = name;
    WINTER_ASSERT(color != glm::vec3(0.0f, 0.0f, 0.0f));
    WINTER_ASSERT(direction != glm::vec3(0.0f, 0.0f, 0.0f));
}

Light::~Light(){
}
void Light::UpdateUniforms(base::Shader *shader){
    shader->SetUniform<float, 3>("u_" + name_ + ".color", color_.x, color_.y, color_.z);
    shader->SetUniform<float, 1>("u_" + name_ + ".ambient_intensity", ambient_intensity_);
    shader->SetUniform<float, 3>("u_" + name_ + ".direction", direction_.x, direction_.y, direction_.z);
    shader->SetUniform<float, 1>("u_" + name_ + ".diffuse_intensity", diffuse_intensity_);
}
void Light::Unbind(){

}

} // namespace util
} // namespace winter
