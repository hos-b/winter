#ifndef __WINTER_OMNI_SHADOW_MAP_H__
#define __WINTER_OMNI_SHADOW_MAP_H__

namespace winter
{
namespace light
{
	
class OmniShadowMap
{
public:
	OmniShadowMap(unsigned int width, unsigned int height);
	OmniShadowMap(const OmniShadowMap &) = delete;
	~OmniShadowMap();
	void BindFramebuffer();
	void UnbindFramebuffer();
	void BindMap(unsigned int texture_unit);

	unsigned int shadow_width();
	unsigned int shadow_height();

private:
	unsigned int fbo_renderer_id_;
	unsigned int map_renderer_id_; 
	unsigned int smap_width_, smap_height_;

	// far plane of the light emission
	float max_travel_distance_;
	bool initialized_;
};

} // namespace base
} // namespace winter


#endif