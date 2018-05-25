#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(VertexBuffer& vbo, VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;

private:
    GLuint _vaoID;

};

#endif