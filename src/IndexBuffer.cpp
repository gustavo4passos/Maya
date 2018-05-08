#include "../include/IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint* data, GLsizeiptr count) {
  glGenBuffers(1, &(this->_iboID));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_iboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &(this->_iboID));
}

void IndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_iboID);
}

void IndexBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
