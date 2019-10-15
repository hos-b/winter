#ifndef __WINTER_glfw_WINDOW_H__
#define __WINTER_glfw_WINDOW_H__

#include "framework/util/debug.h"
#include "framework/base/abstract_types.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

namespace winter
{
namespace util
{

class Window
{
public:
    Window(unsigned int width, unsigned int height, const std::string& name);
    Window();
    ~Window();

    GLFWwindow* glfw_window() { return glfw_window_; }
    unsigned int buffer_width() { return buffer_width_; }
    unsigned int buffer_height() { return buffer_height_; }
    bool is_ok() { return !glfwWindowShouldClose(glfw_window_); }
    float aspect_ratio() { return (float)buffer_width_/(float)buffer_height_; }
    void SwapBuffers() { glfwSwapBuffers(glfw_window_); }
    void PollEvents();
    void RegsiterInputSubscriber(base::InputSubscriber* subscriber, const std::string& name);
    void RemoveInputSubscriber(const std::string& name);
    double delta_time() { return delta_time_.count(); }

private:
    void InitializeGLFW();
    void SetupCallbacks();
    void KeyboardCallback(int key, int code , int action, int mods);
    void MouseMoveCallback(double x, double y);
    void MouseKeyCallback(int button, int action, int mods);

    // window variables
    unsigned int width_, height_;
    GLFWwindow *glfw_window_;
    std::string name_;
    int buffer_width_, buffer_height_;
    // keyboard input
    bool keys_[1024];
    // mouse input
    double last_x_, last_y_;
    double delta_x_, delta_y_;
    bool register_mouse_events_;
    bool hide_cursor_;
    // input subscribers
    std::vector<std::pair<base::InputSubscriber*, std::string>> subscribers_;
    // delta time
    std::chrono::_V2::steady_clock::time_point current_tick_, last_tick_;
    std::chrono::duration<double> delta_time_;
    
};

} // namespace util
} // namespace winter
#endif
