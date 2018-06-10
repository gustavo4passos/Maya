#include "../include/IndexBuffer.h"

#include "../include/GLCall.h"

IndexBuffer::IndexBuffer(const GLuint* data, GLsizeiptr count) {
	GLCall(glGenBuffers(1, &(this->_iboID)));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_iboID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &(this->_iboID)));
}

void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_iboID));
}

void IndexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
