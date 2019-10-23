#include "framework/base/texture.h"
#include "framework/misc/debug.h"

#include <stb_image/stb_image.h>
#include <GL/glew.h>
#include <iostream>
#include <future>

namespace winter
{
namespace base
{

Texture::Texture(const std::string& file_path, unsigned int gl_mode, bool alpha) : file_path_(file_path), local_buffer_(nullptr), width_(0), height_(0), bits_per_pixel_(0)
{
	loaded_ = false;
	GLDebug(glGenTextures(1, &renderer_id_));
	GLDebug(glBindTexture(GL_TEXTURE_2D, renderer_id_));

    stbi_set_flip_vertically_on_load(1);
	std::future<unsigned char*> img_future = std::async(std::launch::async, stbi_load, file_path_.c_str(), &width_, &height_, &bits_per_pixel_, 4);

	GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_mode));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_mode));
	local_buffer_ = img_future.get();
	if(!local_buffer_){
		std::cout << "failed to load texture " << file_path << std::endl;
		loaded_ = false;
		return;
	}

	if(alpha)
		GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer_));
	else
		GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, local_buffer_));
	GLDebug(glGenerateMipmap(GL_TEXTURE_2D));
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
	// free memory
	stbi_image_free(local_buffer_);
	loaded_ = true;
}
Texture::~Texture()
{
    GLDebug(glDeleteTextures(1, &renderer_id_));
    renderer_id_ = 0;
    file_path_ = "";
}

int Texture::width() { return width_; }
int Texture::height() { return height_; }
bool Texture::loaded() { return loaded_; }

void Texture::Bind(unsigned int slot) const
{
    GLDebug(glActiveTexture(GL_TEXTURE0 + slot));
    GLDebug(glBindTexture(GL_TEXTURE_2D, renderer_id_));
}
void Texture::Unbind() const
{
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));
}

} // namespace base
} // namespace winter