#ifndef __WINTER_LIGHT__
#define __WINTER_LIGHT__

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "framework/base/shader.h"
#include "framework/base/shadow_map.h"

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
	// TODO: virtual void InitShadowMap(unsigned int width, unsigned int height) = 0;
	// TODO: virtual glm::mat4 light_transform() const = 0 ;
	// getter|setter
	void SetColor(const glm::vec3& color);
	void SetAmbientIntensity(float ambient_intensity);
	void SetDiffuseIntensity(float diffuse_intensity);
	glm::vec3 color() const;
	float ambient_intensity() const;
	float diffuse_intensity() const;
	base::ShadowMap *shadow_map() const;

protected:
	glm::vec3 color_;
	std::string name_;
	float ambient_intensity_;
    float diffuse_intensity_;

	// shadows
	glm::mat4 light_projection_;
	base::ShadowMap* shadow_map_;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight(const std::string& name, const glm::vec3& color, const glm::vec3& direction,
					 float ambient_intensity, float diffuse_intensity);
    DirectionalLight(const DirectionalLight &) = delete;
	~DirectionalLight();

	void InitShadowMap(unsigned int width, unsigned int height, float cuboid_w, float cuboid_h, float cuboid_d);
	void UpdateUniforms(base::Shader *shader) override;
	// getter|setter
	void SetDirection(const glm::vec3 &direction);
	glm::vec3 direction() const;
	virtual glm::mat4 light_transform() const;

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