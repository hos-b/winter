#include "framework/light/omni_shadow_map.h"
#include "framework/misc/debug.h"
#include <GL/glew.h>
#include <iostream>

namespace winter
{
namespace light
{
OmniShadowMap::OmniShadowMap(unsigned int map_width, unsigned int map_height) : smap_width_(map_width), smap_height_(map_height) {
	glGenFramebuffers(1, &fbo_renderer_id_);
	
	glGenTextures(1, &map_renderer_id_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, map_renderer_id_);
	for (unsigned int i = 0; i < 6; ++i){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, smap_width_, smap_height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_renderer_id_);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, map_renderer_id_, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "framebuffer error " << glCheckFramebufferStatus(GL_FRAMEBUFFER);
		initialized_ = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	initialized_ = true;
}

OmniShadowMap::~OmniShadowMap(){
	UnbindFramebuffer();
	if(fbo_renderer_id_)
		glDeleteFramebuffers(1, &fbo_renderer_id_);
	if(map_renderer_id_)
		glDeleteTextures(1, &map_renderer_id_);
}
void OmniShadowMap::BindFramebuffer(){
	GLDebug(glBindFramebuffer(GL_FRAMEBUFFER, fbo_renderer_id_));
}
void OmniShadowMap::UnbindFramebuffer(){
	GLDebug(glBindFramebuffer(GL_FRAMEBUFFER,0));
}
void OmniShadowMap::BindMap(unsigned int texture_unit){
	GLDebug(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLDebug(glBindTexture(GL_TEXTURE_CUBE_MAP, map_renderer_id_));
}
unsigned int OmniShadowMap::shadow_width() { return smap_width_; }
unsigned int OmniShadowMap::shadow_height() { return smap_height_; }

} // namespace base
} // namespace winter
