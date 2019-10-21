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
void Light::SetColor(const glm::vec3& color) { color_ = color;}
void Light::SetAmbientIntensity(float ambient_intensity) { ambient_intensity_ = ambient_intensity; }
void Light::SetDiffuseIntensity(float diffuse_intensity) { diffuse_intensity_ = diffuse_intensity; }
glm::vec3 Light::color() const { return color_; }
float Light::ambient_intensity() const { return ambient_intensity_; }
float Light::diffuse_intensity() const { return diffuse_intensity_; }
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
glm::vec3 DirectionalLight::direction() const { return direction_; }
// Point Light -------------------------------------------------------------------------------------
unsigned int PointLight::count_ = 0;
PointLight::~PointLight(){
	PointLight::count_--;
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
PointLight::PointLight(const std::string &name, const glm::vec3 &color, const glm::vec3 &position, float ambient_intensity,
					   float diffuse_intensity) : Light(name, color, ambient_intensity, diffuse_intensity), position_(position)
{
	exponent_ = 0.2f;
	linear_ = 0.5f;
	constant_ = 1.0f;
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
	shader->SetUniform<int, 1>("u_plight_count", PointLight::count_);
}
void PointLight::SetFadeParameters(float constant, float linear, float exponent){
	exponent_ = exponent;
	linear_ = linear;
	constant_ = constant;
}
void PointLight::SetPosition(const glm::vec3& position){ position_ = position; }
std::tuple<float, float, float> PointLight::GetFadeParameters(){ return std::make_tuple(exponent_, linear_, constant_); }
glm::vec3 PointLight::position() const { return position_;}
unsigned int PointLight::count() { return count_; }
// Spot Light --------------------------------------------------------------------------------------
unsigned int SpotLight::count_ = 0;
SpotLight::SpotLight() : Light()
{
	position_ = glm::vec3(1.0f, 1.0f, 1.0f);
	direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
	cutoff_angle_ = 0.0f;
	coefficient_ = cosf(glm::radians(cutoff_angle_));
	exponent_ = 0.0f;
	linear_ = 0.0f;
	constant_ = 1.0f;
	index_ = SpotLight::count_;
	SpotLight::count_++;
	std::cout << "added new SpotLight[" << index_ << "]  total:" << count_ << std::endl;
}
SpotLight::SpotLight(const std::string &name, const glm::vec3 &color, const glm::vec3 position, const glm::vec3 &direction,
					 float cutoff_angle, float ambient_intensity, float diffuse_intensity) : 
					 Light(name, color, ambient_intensity, diffuse_intensity), position_(position), direction_(glm::normalize(direction))
{
	exponent_ = 0.2f;
	linear_ = 0.5f;
	constant_ = 1.0f;
	cutoff_angle_ = cutoff_angle;
	coefficient_ = cosf(glm::radians(cutoff_angle_));
	index_ = SpotLight::count_;
	SpotLight::count_++;
	std::cout << "added new SpotLight[" << index_ << "]  total:" << count_ << std::endl;
}
SpotLight::~SpotLight(){
	SpotLight::count_--;
}
void SpotLight::UpdateUniforms(base::Shader *shader){
	std::string index = std::to_string(index_);
	shader->SetUniform<float, 3>("u_slight[" + index + "].base.base.color", color_.x, color_.y, color_.z);
	shader->SetUniform<float, 1>("u_slight[" + index + "].base.base.ambient_intensity", ambient_intensity_);
    shader->SetUniform<float, 1>("u_slight[" + index + "].base.base.diffuse_intensity", diffuse_intensity_);
    shader->SetUniform<float, 3>("u_slight[" + index + "].base.position", position_.x, position_.y, position_.z);
	shader->SetUniform<float, 1>("u_slight[" + index + "].base.constant", constant_);
	shader->SetUniform<float, 1>("u_slight[" + index + "].base.linear", linear_);
	shader->SetUniform<float, 1>("u_slight[" + index + "].base.exponent", exponent_);
	shader->SetUniform<float, 3>("u_slight[" + index + "].direction",direction_.x, direction_.y, direction_.z);
	shader->SetUniform<float, 1>("u_slight[" + index + "].coeff", coefficient_);
	// sending the count
	shader->SetUniform<int, 1>("u_slight_count", SpotLight::count_);
}
void SpotLight::SetFadeParameters(float constant, float linear, float exponent){
	exponent_ = exponent;
	linear_ = linear;
	constant_ = constant;
}
void SpotLight::SetCutoffAngle(float cutoff_angle){ 
	cutoff_angle_ = cutoff_angle;
	coefficient_ = cosf(glm::radians(cutoff_angle_));
}
void SpotLight::SetPosition(const glm::vec3 &position){ position_ = position; }
void SpotLight::SetDirection(const glm::vec3 &direction){ direction_ = glm::normalize(direction); }
std::tuple<float, float, float> SpotLight::GetFadeParameters(){ return std::make_tuple(exponent_, linear_, constant_); }
float SpotLight::cutoff_angle() const { return cutoff_angle_; }
glm::vec3 SpotLight::position() const { return position_; }
glm::vec3 SpotLight::direction() const { return direction_; }
unsigned int SpotLight::count(){ return count_; }

} // namespace util
} // namespace winter
