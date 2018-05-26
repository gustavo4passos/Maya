#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <GL/glew.h>
#include <GL/gl.h>

class VertexBuffer {
public:
  VertexBuffer(const GLvoid* data, GLsizeiptr size);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;

private:
  GLuint _vboID;
};

#endif
