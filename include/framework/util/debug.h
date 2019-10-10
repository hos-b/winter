#ifndef __GL_DEBUG__
#define __GL_DEBUG__

#include <string>

#define ASSERT(x) if (!(x)) __builtin_trap()

// remove for release
#define DEBUG_FLAG

#ifdef DEBUG_FLAG
#define GLDebug(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLDebug(x) x;
#endif



void GLClearError();
bool GLLogCall(const char* function_name, const char* file_name, int line);
std::string GLTranslateError(unsigned int gl_error);

#endif