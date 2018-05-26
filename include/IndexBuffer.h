#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <GL/glew.h>
#include <GL/gl.h>

class IndexBuffer {
public:
  IndexBuffer(const GLuint* data, GLsizeiptr count);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

  inline unsigned int GetCount() const { return _count; }

private:
  GLuint _iboID;
  GLsizeiptr _count;
};

#endif
