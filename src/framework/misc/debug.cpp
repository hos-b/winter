#include "framework/misc/debug.h"
#include <GL/glew.h>

#include <iostream>
#include <sstream>

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char *function_name, const char* file, int line)
{
    if(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error " << GLTranslateError(error) << "] at " << function_name <<
           " in " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

std::string GLTranslateError(unsigned int gl_error)
{
    std::string result = "UNKNOWN ERROR";
    switch (gl_error)
    {
    case GL_INVALID_ENUM:
        result = "GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        result = "GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION :
        result = "GL_INVALID_OPERATION";
        break;
    case GL_STACK_OVERFLOW :
        result = "GL_STACK_OVERFLOW";
        break;
    case GL_STACK_UNDERFLOW :
        result = "GL_STACK_UNDERFLOW";
        break;
    case GL_OUT_OF_MEMORY :
        result = "GL_OUT_OF_MEMORY";
        break;
    default:
        std::stringstream stream;
        stream << std::hex << gl_error;
        result = stream.str();
        break;
    }
    return result;
}
