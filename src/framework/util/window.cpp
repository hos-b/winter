#include <fstream>
#include <streambuf>
#include "framework/util/window.h"

namespace winter
{
namespace util
{
Window::Window(unsigned int width, unsigned int height, const std::string& name) : width_(width), height_(height), glfw_window_(nullptr), name_(name)
{
    InitializeGLFW();
    SetupCallbacks();
}
Window::Window() : width_(800), height_(600), glfw_window_(nullptr), name_("glFramework") 
{
    InitializeGLFW();
    SetupCallbacks();
}
Window::~Window() 
{
    glfwTerminate();
}

void Window::InitializeGLFW()
{
    // glfw window setup
    if (!glfwInit())
        std::cout << "glfw init failed" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfw_window_ = glfwCreateWindow(width_, height_, name_.c_str() , NULL, NULL);
    if (!glfw_window_)
    {
        glfwTerminate();
        std::cout << "failed to create window" << std::endl;
    }
    glfwGetFramebufferSize(glfw_window_, &buffer_width_, &buffer_height_);
    glfwMakeContextCurrent(glfw_window_);
    // GLEW init
    glewExperimental = GL_TRUE; 
    GLenum init_enum = glewInit();
    if (init_enum!= GLEW_OK)
        std::cout << glewGetErrorString(init_enum) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
}
void Window::RegsiterInputSubscriber(base::InputSubscriber* subscriber, const std::string& name)
{ 
    subscribers_.emplace_back(std::make_pair(subscriber, name));
    std::cout<< "added " << name << " to subscrbers (" << subscribers_.size() << ")" << std::endl;
}
void Window::RemoveInputSubscriber(const std::string& name)
{
    for(auto it_ = subscribers_.begin(); it_ != subscribers_.end(); it_++)
    {
        if(it_->second == name)
        {
            subscribers_.erase(it_);
            std::cout<< "removed " << name << " from subscrbers (" << subscribers_.size() << ")" << std::endl;
            break;
        }
    }
}

void Window::SetupCallbacks()
{
    glfwSetWindowUserPointer(glfw_window_, this);
    // set up keyboard callback
    auto keyboard_lambda = [](GLFWwindow* w, int key, int code , int action, int mode) {
        static_cast<Window*>(glfwGetWindowUserPointer(w))->KeyboardCallback(key, code, action, mode);
    };
    auto mouse_move_lambda = [] (GLFWwindow* w, double x, double y){
        static_cast<Window*>(glfwGetWindowUserPointer(w))->MouseMoveCallback(x, y);
    };
    glfwSetKeyCallback(glfw_window_, keyboard_lambda);
    glfwSetCursorPosCallback(glfw_window_, mouse_move_lambda);
    // glfwSetMouseButtonCallback(glfw_window_, mouse_button_lambda);
    for (unsigned short i=0; i<1024; i++)
    {
        keys_[i] = false;
    }

    hide_cursor_ = register_mouse_events_ = false;
}

void Window::KeyboardCallback(int key, int code , int action, int mods)
{
    //silencing compiler
    (void)code;
    (void)mods;

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(glfw_window_, GL_TRUE);
    
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        hide_cursor_ = !hide_cursor_;
        glfwSetInputMode(glfw_window_, GLFW_CURSOR, (hide_cursor_ ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
    }

    if(key >= 0 && key <= 1024)
    {
        if(action == GLFW_PRESS)
            keys_[key] = true;
        else if(action == GLFW_RELEASE)
            keys_[key] = false;
    }
    for (auto& sub : subscribers_)
    {
        sub.first->OnKeyboardEvent(key, code, action, mods);
    }
}
void Window::MouseMoveCallback(double x, double y)
{
    if(!register_mouse_events_)
    {
        delta_x_ = 0;
        delta_y_ = 0;
        register_mouse_events_ = true;
    }
    else
    {
        delta_x_ = x - last_x_;
        delta_y_ = last_y_ - y;
    }
    last_x_ = x;
    last_y_ = y;
    for (auto& sub : subscribers_)
    {
        sub.first->OnMouseMoveEvent(delta_x_, delta_y_);
    }
}

void Window::MouseKeyCallback(int button, int action, int mods)
{
    for (auto& sub : subscribers_)
    {
        sub.first->OnMouseKeyEvent(button, action, mods);
    }
}

} // namespace util
} // namespace winter

