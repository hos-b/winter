#include "framework/base/shader.h"

#ifndef __WINTER_MATERIAL_H__
#define	__WINTER_MATERIAL_H__

namespace winter
{
namespace mesh
{
	
class Material
{
public:
	Material(std::string name, float specular_intensity, float shininess);
	Material(const Material &mat) = delete;
	~Material();

	void UpdateUnifrorms(base::Shader* shader);

private:
	float specular_intensity_;	// defines how bright the color is
	float shininess_;			// defines how focused the spot is (object smoothness/shininess)

	std::string name_;
};

} // namespace mesh
} // namespace winter



#endif