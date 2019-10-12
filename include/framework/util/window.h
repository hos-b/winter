#ifndef __WINTER_WINDOW_H__
#define __WINTER_WINDOW_H__

#include "framework/util/debug.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <string>

namespace winter
{
namespace util
{

class Window
{
public:
    Window(unsigned int width, unsigned int height, const std::string& name) : width_(width), height_(height), window_(nullptr), name_(name)
    { Initialize(); }
    Window() : width_(800), height_(600), window_(nullptr), name_("glFramework") { Initialize(); }
    ~Window() { glfwTerminate(); }

    inline GLFWwindow* window() { return window_; }
    inline unsigned int buffer_width() { return buffer_width_; }
    inline unsigned int buffer_height() { return buffer_height_; }
    inline bool is_ok() { return !glfwWindowShouldClose(window_); }
    inline void SwapBuffers() { glfwSwapBuffers(window_); }
    inline void PollEvents() { glfwPollEvents(); }
    inline float aspect_ratio() { return (float)buffer_width_/(float)buffer_height_; }

private:
    void Initialize();
    unsigned int width_, height_;
    std::string name_;
    GLFWwindow *window_;
    int buffer_width_, buffer_height_;
};

} // namespace util
} // namespace winter
#endif
