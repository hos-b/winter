#include "framework/mesh/material.h"

namespace winter
{
namespace mesh
{
	
Material::Material(std::string name, float specular_intensity, float shininess)
{
	specular_intensity_ = specular_intensity;
	shininess_ = shininess;
	name_ = name;
}
void Material::UpdateUnifrorms(base::Shader* shader)
{
	shader->SetUniform<float, 1>("u_" + name_ + ".specular_intensity", specular_intensity_);
	shader->SetUniform<float, 1>("u_" + name_ + ".shininess", shininess_);
}
Material::~Material()
{
}

} // namespace mesh
} // namespace winter
