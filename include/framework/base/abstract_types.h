#ifndef __WINTER_ABSTRACT_TYPES__
#define __WINTER_ABSTRACT_TYPES__

namespace winter
{
namespace base
{

class Renderable
{
public:
    virtual ~Renderable() {};
    virtual void OnRender() = 0;
};

} // namespace base
} // namespace winter
#endif