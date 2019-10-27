#ifndef __WINTER_SHADOW_MAP_H__
#define __WINTER_SHADOW_MAP_H__

#include <GL/glew.h>

namespace winter
{
namespace light
{

class ShadowMap
{
public:
	ShadowMap(unsigned int smap_width, unsigned int smap_height);
	ShadowMap(const ShadowMap &) = delete;
	~ShadowMap();

	void BindFramebuffer();
	void UnbindFramebuffer();
	void BindMap(unsigned int texture_unit);
	unsigned int shadow_width();
	unsigned int shadow_height();

private:
	unsigned int fbo_renderer_id_;
	unsigned int map_renderer_id_; 
	unsigned int smap_width_, smap_height_;
	bool initialized_;
};

} // namespace base
} // namspace winter

#endif