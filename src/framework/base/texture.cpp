#include "framework/base/texture.h"
#include "framework/misc/debug.h"

#include <stb_image/stb_image.h>
#include <future>
#include <GL/glew.h>

namespace winter
{
namespace base
{

Texture::Texture(const std::string& file_path, unsigned int gl_mode) : file_path_(file_path), local_buffer_(nullptr), width_(0), height_(0), bits_per_pixel_(0)
{
    GLDebug(glGenTextures(1, &renderer_id_));
    GLDebug(glBindTexture(GL_TEXTURE_2D, renderer_id_));

    stbi_set_flip_vertically_on_load(1);
	std::future<unsigned char*> img_future = std::async(std::launch::async, stbi_load, file_path_.c_str(), &width_, &height_, &bits_per_pixel_, 4);
	local_buffer_ = stbi_load(file_path_.c_str(), &width_, &height_, &bits_per_pixel_, 4);

	GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_mode));
    GLDebug(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_mode));
	local_buffer_ = img_future.get();
	GLDebug(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer_));
	GLDebug(glGenerateMipmap(GL_TEXTURE_2D));
    GLDebug(glBindTexture(GL_TEXTURE_2D, 0));

    if(local_buffer_)
        stbi_image_free(local_buffer_);
}
Texture::~Texture()
{
    GLDebug(glDeleteTextures(1, &renderer_id_));
    renderer_id_ = 0;
    file_path_ = "";
}

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