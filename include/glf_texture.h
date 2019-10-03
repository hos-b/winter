#ifndef __GL_TEXTURE_H__
#define __GL_TEXTURE_H__

#include <string>

class Texture
{
public:
    Texture(const std::string& file_path, unsigned int gl_mode);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int width() { return width_; }
    inline int height() { return height_; } 

private:
    unsigned int renderer_id_;
    std::string file_path_;
    unsigned char* local_buffer_;
    int width_, height_, bits_per_pixel_;
};

#endif