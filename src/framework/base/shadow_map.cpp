#include "framework/base/shadow_map.h"
#include "framework/misc/debug.h"
#include <iostream>

namespace winter
{
namespace base
{
	
ShadowMap::ShadowMap(unsigned int smap_width, unsigned int smap_height) : smap_width_(smap_width), smap_height_(smap_height)
{
	glGenFramebuffers(1, &fbo_renderer_id_);

	glGenTextures(1, &map_renderer_id_);
	glBindTexture(GL_TEXTURE_2D, map_renderer_id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, smap_width_, smap_height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LINEAR
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_renderer_id_);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, map_renderer_id_, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "framebuffer error " << status;
		initialized_ = false;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	initialized_ = true;
}
ShadowMap::~ShadowMap(){
	UnbindFramebuffer();
	if(fbo_renderer_id_)
		glDeleteFramebuffers(1, &fbo_renderer_id_);
	if(map_renderer_id_)
		glDeleteTextures(1, &map_renderer_id_);
}
void ShadowMap::BindFramebuffer(){
	GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, fbo_renderer_id_));
}
void ShadowMap::UnbindFramebuffer(){
	GLDebug(glBindFramebuffer(GL_FRAMEBUFFER,0));
}
void ShadowMap::BindMap(unsigned int texture_unit){
	GLDebug(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLDebug(glBindTexture(GL_TEXTURE_2D, map_renderer_id_));
}
unsigned int ShadowMap::shadow_width() { return smap_width_; }
unsigned int ShadowMap::shadow_height() { return smap_height_; }

} // namespace base
} // namespace winter
