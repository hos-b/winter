#ifndef __WINTER_TEXTURE_H__
#define __WINTER_TEXTURE_H__

#include <string>

namespace winter
{
namespace base
{

class Texture
{
public:
    Texture(const std::string& file_path, unsigned int gl_mode, bool alpha=true);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int width();
	int height();
	bool loaded();

private:
    unsigned int renderer_id_;
    std::string file_path_;
	bool loaded_;
	unsigned char *local_buffer_;
	int width_, height_, bits_per_pixel_;
};

} // namespace base
} // namespace winter
#endif