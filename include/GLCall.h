#include <iostream>

#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>

#define M_DEBUG

#if defined(__GNUC__)
#define DEBUG_BREAK() __builtin_trap()
#elif defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#endif

#if defined(M_DEBUG)
#define ASSERT(x) if(!x) DEBUG_BREAK()
#else
#define ASSERT(x) x
#endif

static void GLClearError() { while(glGetError() != GL_NO_ERROR); }

static bool GLLogCall(const char* function, const char* file, int line) { 
    while(GLenum error = glGetError()) {
        std::cerr << "OpenGL Error: " << file << ":" << line << " " << function << " [" << gluErrorString(error) << "]\n"; 
        return false;
    }

    return true;
}

#if defined(M_DEBUG)
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif
