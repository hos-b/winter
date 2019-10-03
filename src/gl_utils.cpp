#include "gl_utils.h"
#include <fstream>
#include <streambuf>

GLFWwindow* BoilerPlate(unsigned int width, unsigned int height)
{
    // GLFW init
    GLFWwindow* window;
    if (!glfwInit())
        std::cout << "glfw init failed" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "failed to create window" << std::endl;
    }
    glfwMakeContextCurrent(window);
    
    // GLEW init
    glewExperimental = GL_TRUE; 
    GLenum init_enum = glewInit();
    if (init_enum!= GLEW_OK)
        std::cout << glewGetErrorString(init_enum) << std::endl;

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    return window;
}