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

class Light{
public:
	Light();
	Light(const Light &) = delete;
	Light(const std::string &name, const glm::vec3 &color, float ambient_intensity, float diffuse_intensity);
	virtual ~Light() {}
	virtual void UpdateUniforms(base::Shader *shader) = 0;
	void SetColor(glm::vec3 color) { color_ = color;}
	void SetAmbientIntensity(float ambient_intensity) { ambient_intensity_ = ambient_intensity; }
	void SetDiffuseIntensity(float diffuse_intensity) { diffuse_intensity_ = diffuse_intensity; }
	glm::vec3 color() { return color_; }
	float ambient_intensity() { return ambient_intensity_; }
	float diffuse_intensity() { return diffuse_intensity_; }

protected:
	void UpdateSharedUniforms(base::Shader *shader);

	glm::vec3 color_;
	std::string name_;
	float ambient_intensity_;
    float diffuse_intensity_;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight(const std::string& name, const glm::vec3& color, const glm::vec3& direction,
					 float ambient_intensity, float diffuse_intensity);
    DirectionalLight(const DirectionalLight &) = delete;
	~DirectionalLight();

	void UpdateUniforms(base::Shader *shader) override;
	glm::vec3 direction() { return direction_; }

private:
    glm::vec3 direction_;
};

class PointLight : public Light{
public:
	PointLight();
	PointLight(const std::string& name, const glm::vec3& color, const glm::vec3& position, 
			   float ambient_intensity, float diffuse_intensity);
	PointLight(const PointLight &) = delete;
	~PointLight();

	void UpdateUniforms(base::Shader *shader) override;
	void SetPosition(const glm::vec3& position);
	void SetFadingParameters(float constant, float linear, float exponent);
	glm::vec3 position() { return position_; }

	static unsigned int count();

private:
	// keeping track of PointLights for the shader
	static unsigned int count_;
	// index of the light in the shader
	unsigned int index_;
	glm::vec3 position_;
	float constant_, linear_, exponent_;
};

class SpotLight : public Light{
public:
	SpotLight(const std::string& name, const glm::vec3& color, glm::vec3 position, float cutoff_angle,
			  float ambient_intensity, float diffuse_intensity);
    SpotLight(const SpotLight &) = delete;
	~SpotLight();

	void UpdateUniforms(base::Shader *shader) override;
	void SetCutoffAngle(float cutoff_angle);
	void SetPosition(const glm::vec3 &position);
	float cutoff_angle() { return cutoff_angle_; }
	glm::vec3 position() { return position_; }

private:
	// keeping track of PointLights for the shader
	static unsigned int count_;
	// index of the light in the shader
	unsigned int index_;
	glm::vec3 position_;
	float cutoff_angle_;
};

} // namespace util
} // namespace winter
#endif