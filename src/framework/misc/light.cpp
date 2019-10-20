#include "framework/misc/light.h"
#include "framework/misc/debug.h"
#include <iostream>

namespace winter
{
namespace util
{
// Light Constructors -----------------------------------------------------------------------------------
Light::Light(){
	name_ = "light";
	color_ = glm::vec3(1.0f, 1.0f, 1.0f);
	ambient_intensity_ = 0.5f;
	diffuse_intensity_ = 0.5f;
}
Light::Light(const std::string& name, const glm::vec3& color, float ambient_intensity, float diffuse_intensity) {
	name_ = name;
	color_ = color;
	ambient_intensity_ = ambient_intensity;
	diffuse_intensity_ = diffuse_intensity;
	WINTER_ASSERT(color != glm::vec3(0.0f, 0.0f, 0.0f));
	WINTER_ASSERT(ambient_intensity >= 0.0f && ambient_intensity <= 1.0f);
	WINTER_ASSERT(diffuse_intensity >= 0.0f && diffuse_intensity <= 1.0f);
}
// Directional Light -------------------------------------------------------------------------------------
DirectionalLight::~DirectionalLight() {}
DirectionalLight::DirectionalLight() :  Light()
{
    direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
}
DirectionalLight::DirectionalLight(const std::string& name, const glm::vec3& color, const glm::vec3& direction,
	float ambient_intensity, float diffuse_intensity) : Light(name, color, ambient_intensity, diffuse_intensity)
{
    WINTER_ASSERT(direction != glm::vec3(0.0f, 0.0f, 0.0f));
    direction_ = glm::normalize(direction);
}

void DirectionalLight::UpdateUniforms(base::Shader *shader){
	shader->SetUniform<float, 3>("u_" + name_ + ".base.color", color_.x, color_.y, color_.z);
    shader->SetUniform<float, 1>("u_" + name_ + ".base.ambient_intensity", ambient_intensity_);
    shader->SetUniform<float, 1>("u_" + name_ + ".base.diffuse_intensity", diffuse_intensity_);
	shader->SetUniform<float, 3>("u_" + name_ + ".direction", direction_.x, direction_.y, direction_.z);
}

// Point Light -------------------------------------------------------------------------------------
unsigned int PointLight::count_ = 0;
PointLight::~PointLight(){
	count_--;
}
PointLight::PointLight() : Light()
{
	position_ = glm::vec3(1.0f, 1.0f, 1.0f);
	exponent_ = 0.0f;
	linear_ = 0.0f;
	constant_ = 1.0f;
	index_ = PointLight::count_;
	PointLight::count_++;
	std::cout << "added new PointLight[" << index_ << "]  total:" << count_ << std::endl;
}
PointLight::PointLight(const std::string& name, const glm::vec3& color, const glm::vec3& position, 
					   float ambient_intensity, float diffuse_intensity) : Light(name, color, ambient_intensity, diffuse_intensity)
{
	exponent_ = 0.2f;
	linear_ = 0.5f;
	constant_ = 1.0f;
	position_ = position;
	index_ = PointLight::count_;
	PointLight::count_++;
	std::cout << "added new PointLight[" << index_ << "]  total:" << count_ << std::endl;
}

void PointLight::UpdateUniforms(base::Shader *shader){
	std::string index = std::to_string(index_);
	shader->SetUniform<float, 3>("u_plight[" + index + "].base.color", color_.x, color_.y, color_.z);
	shader->SetUniform<float, 1>("u_plight[" + index + "].base.ambient_intensity", ambient_intensity_);
    shader->SetUniform<float, 1>("u_plight[" + index + "].base.diffuse_intensity", diffuse_intensity_);
    shader->SetUniform<float, 3>("u_plight[" + index + "].position", position_.x, position_.y, position_.z);
	shader->SetUniform<float, 1>("u_plight[" + index + "].constant", constant_);
	shader->SetUniform<float, 1>("u_plight[" + index + "].linear", linear_);
	shader->SetUniform<float, 1>("u_plight[" + index + "].exponent", exponent_);
	// sending the count
	shader->SetUniform<int, 1>("u_plight_count", count_);
}
void PointLight::SetPosition(const glm::vec3& position){
	position_ = position;
}
void PointLight::SetFadingParameters(float constant, float linear, float exponent){
	exponent_ = exponent;
	linear_ = linear;
	constant_ = constant;
}

// Spot Light --------------------------------------------------------------------------------------
unsigned int SpotLight::count_ = 0;


} // namespace util
} // namespace winter
