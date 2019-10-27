#ifndef __WINTER_SKYBOX_H__
#define __WINTER_SKYBOX_H__

#include "framework/base/shader.h"
#include "framework/mesh/mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <vector>


namespace winter
{
namespace mesh
{
	class Skybox
	{
	public:
		Skybox(const std::vector<std::string>& file_paths);
		Skybox(const Skybox&) = delete;
		~Skybox();

		void RenderSkybox(glm::mat4 camera_view, glm::mat4 projection_matrix);


	private:
		Mesh *sky_mesh_;
		base::Shader *sky_shader_;
		unsigned int texture_id_;
	};
} // namespace mesh
} // namespace winter


#endif
