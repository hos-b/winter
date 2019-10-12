#include <fstream>
#include <streambuf>
#include "framework/util/window.h"

namespace winter
{
namespace util
{

void Window::Initialize()
{
    if (!glfwInit())
        std::cout << "glfw init failed" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width_, height_, name_.c_str() , NULL, NULL);
    if (!window_)
    {
        glfwTerminate();
        std::cout << "failed to create window" << std::endl;
    }
    glfwGetFramebufferSize(window_, &buffer_width_, &buffer_height_);
    glfwMakeContextCurrent(window_);
    // GLEW init
    glewExperimental = GL_TRUE; 
    GLenum init_enum = glewInit();
    if (init_enum!= GLEW_OK)
        std::cout << glewGetErrorString(init_enum) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}

} // namespace util
} // namespace winter

