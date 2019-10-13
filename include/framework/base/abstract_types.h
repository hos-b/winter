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

class InputSubscriber
{
public:
    virtual ~InputSubscriber() {};
    virtual void OnKeyboardEvent(int key, int code , int action, int mods) = 0;
    virtual void OnMouseKeyEvent(int button, int action, int mods) = 0;
    virtual void OnMouseMoveEvent(double delta_x, double delta_y) = 0;
};

} // namespace base
} // namespace winter
#endif