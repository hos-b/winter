#include "gl_utils.h"
#include <fstream>
#include <streambuf>

GLFWwindow* BoilerPlate()
{
    // GLFW init
    GLFWwindow* window;
    if (!glfwInit())
        std::cout << "glfw init failed" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
static unsigned int CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // = &source[0]
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*) alloca(length*sizeof(char)); // dynamic stack memory
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile " << (type==GL_VERTEX_SHADER ? "vertex shader":"fragment shader") << std::endl;
        std::cout << message;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
unsigned int CreateShaders(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    // compiling shaders
    unsigned int vs = CompileShader(vertex_shader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);
    // linking shaders
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    // shaders are attached now, we can delete intermediates
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
std::string ReadShaderFromFile(const std::string& filename)
{
    std::ifstream t(filename);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
}
void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char *function_name, const char* file, int line)
{
    if(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error " << error << "] at " << function_name <<
           " in " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
