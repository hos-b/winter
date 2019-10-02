#ifndef _GL_UTILS__H_
#define _GL_UTILS__H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>



GLFWwindow* BoilerPlate();
std::string ReadShaderFromFile(const std::string&);
std::string ReadShaderFromFile(const std::string&);
unsigned int CreateShaders(const std::string&, const std::string&);


#endif