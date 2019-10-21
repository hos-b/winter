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
	// getter|setter
	void SetColor(const glm::vec3& color);
	void SetAmbientIntensity(float ambient_intensity);
	void SetDiffuseIntensity(float diffuse_intensity);
	glm::vec3 color() const;
	float ambient_intensity() const;
	float diffuse_intensity() const;

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
	// getter|setter
	void SetDirection(const glm::vec3 &direction);
	glm::vec3 direction() const;

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
	void SetFadeParameters(float constant, float linear, float exponent);
	std::tuple<float, float, float> GetFadeParameters();
	glm::vec3 position() const;
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
	SpotLight();
	SpotLight(const std::string &name, const glm::vec3 &color, const glm::vec3 position,
			  const glm::vec3 &direction, float cutoff_angle, float ambient_intensity, float diffuse_intensity);
	SpotLight(const SpotLight &) = delete;
	~SpotLight();

	void UpdateUniforms(base::Shader *shader) override;
	// getter|setter
	void SetCutoffAngle(float cutoff_angle);
	void SetPosition(const glm::vec3 &position);
	void SetDirection(const glm::vec3 &direction);
	void SetFadeParameters(float constant, float linear, float exponent);
	std::tuple<float, float, float> GetFadeParameters();
	float cutoff_angle() const;
	glm::vec3 position() const;
	glm::vec3 direction() const;

	static unsigned int count();
private:
	// keeping track of PointLights for the shader
	static unsigned int count_;
	// index of the light in the shader
	unsigned int index_;
	glm::vec3 position_, direction_;
	// parameters
	float cutoff_angle_;
	float constant_, linear_, exponent_;
	// processed cutoff_angle
	float coefficient_;
};

} // namespace util
} // namespace winter
#endif