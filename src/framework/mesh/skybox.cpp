#include "framework/mesh/skybox.h"

#include <stb_image/stb_image.h>
#include <GL/glew.h>
#include <iostream>
#include <future>

namespace winter
{
namespace mesh
{
/* skybox construct : images should be R, L, U, D, B, F*/
Skybox::Skybox(const std::vector<std::string>& file_paths)
{
	// shader
	sky_shader_ = new base::Shader("../res/shaders/advanced/standard/skybox.glsl");
	// textures
	glGenTextures(1, &texture_id_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
	std::vector<std::future<unsigned char*>>  async_loaders;
	int width, height, bits_per_pixel;
	stbi_set_flip_vertically_on_load(0);
	for (unsigned int i = 0; i < 6; ++i)
		async_loaders.emplace_back(std::async(std::launch::async, stbi_load, file_paths[i].c_str(), &width, &height, &bits_per_pixel, 0));
	for (unsigned int i = 0; i < 6; ++i){
		unsigned char *texture_data = async_loaders[i].get();
		if(!texture_data)
			std::cout << "failed to load texture " << file_paths[i] << std::endl;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
		stbi_image_free(texture_data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// mesh
	unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	float vertices[] = {
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};
	sky_mesh_ = new Mesh("skybox");
	sky_mesh_->AddVertexBufferElement<float>(3);
	sky_mesh_->AddVertexBufferElement<float>(2);
	sky_mesh_->AddVertexBufferElement<float>(3);
	sky_mesh_->CreateMesh(vertices, 64 * sizeof(float), indices, 36);
}

Skybox::~Skybox()
{
	delete sky_mesh_;
	delete sky_shader_;
	glDeleteTextures(1, &texture_id_);
}

/* renders the skybox. remember to bind your shader after this call since the skybox uses a local one*/
void Skybox::RenderSkybox(glm::mat4 view_matrix, glm::mat4 projection_matrix){
	glDepthMask(GL_FALSE);
	glm::mat4 new_view = glm::mat4(glm::mat3(view_matrix));
	sky_shader_->Bind();
	sky_shader_->SetUniform<int, 1>("u_skybox", 0);
	sky_shader_->SetUniform<float *, 16>("u_view", &new_view[0][0]);
	sky_shader_->SetUniform<float *, 16>("u_projection", &projection_matrix[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
	sky_mesh_->OnRender();
	glDepthMask(GL_TRUE);
}

} // namespace mesh
} // namespace winter
