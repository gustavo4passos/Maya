#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>

#include <GL/gl.h>

#include "ErrorHandler.h"

struct VertexBufferLayoutElement {
    GLenum type;
    GLuint count;
    GLboolean normalized;

    static unsigned int GetSizeOfType(GLenum type) {
        switch(type){
            case GL_FLOAT:           return 4;
            case GL_UNSIGNED_INT:    return 4;
            case GL_UNSIGNED_BYTE:   return 1;
            case GL_INT:             return 4;
            case GL_BYTE:            return 1;
        }

        LOG_ERROR("Invalid type.\n");
        return 0;
    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout()
        : _stride(0) { }

    // Specifying the template is mandatory
    template<typename T>
    void Push(GLuint count){ DEBUG_BREAK(); }

     inline const std::vector<VertexBufferLayoutElement>& elements() const { return _elements; }
     inline GLint stride() const { return _stride; }

private:
    std::vector<VertexBufferLayoutElement> _elements;
    GLsizei _stride;
};

/* Template specializations */
template<>
inline void VertexBufferLayout::Push<GLfloat>(GLuint count){
    VertexBufferLayoutElement element;
    element.type = GL_FLOAT;
    element.count = count;
    element.normalized = GL_FALSE;

    _elements.push_back(element);
    _stride += sizeof(GLfloat) * count;
}

template<>
inline void VertexBufferLayout::Push<GLuint>(GLuint count){
    VertexBufferLayoutElement element;
    element.type = GL_UNSIGNED_INT;
    element.count = count;
    element.normalized = GL_FALSE;

    _elements.push_back(element);
    _stride += sizeof(GLuint) * count;
}

template<>
inline void VertexBufferLayout::Push<GLubyte>(GLuint count){
    VertexBufferLayoutElement element;
    element.type = GL_UNSIGNED_BYTE;
    element.count = count;
    element.normalized = GL_FALSE;
    
    _elements.push_back(element);
    _stride += sizeof(GLubyte) * count;
}

template<>
inline void VertexBufferLayout::Push<GLint>(GLuint count){
    VertexBufferLayoutElement element;
    element.type = GL_INT;
    element.count = count;
    element.normalized = GL_FALSE;

    _elements.push_back(element);
    _stride += sizeof(GLint) * count;
}

template<>
inline void VertexBufferLayout::Push<GLbyte>(GLuint count){
    VertexBufferLayoutElement element;
    element.type = GL_BYTE;
    element.count = count;
    element.normalized = GL_FALSE;

    _elements.push_back(element);
    _stride += sizeof(GL_BYTE) * count;
}

#endif
