#include "../include/VertexBuffer.h"

VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size) : _vboID(0) {
  glGenBuffers(1, &(this->_vboID));
  glBindBuffer(GL_ARRAY_BUFFER, this->_vboID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &(this->_vboID));
}

void VertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, this->_vboID);
}

void VertexBuffer::Unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
