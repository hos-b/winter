#ifndef _GL_UTILS__H_
#define _GL_UTILS__H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sys/signal.h>

#define ASSERT(x) if (!(x)) __builtin_trap()
#define GLDebug(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
// raise(SIGTRAP)

GLFWwindow* BoilerPlate();
std::string ReadShaderFromFile(const std::string&);
std::string ReadShaderFromFile(const std::string&);
unsigned int CreateShaders(const std::string&, const std::string&);
void GLClearError();
bool GLLogCall(const char *, const char*, int);

#endif