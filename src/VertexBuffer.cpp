#include "../include/VertexBuffer.h"

#include "../include/GLCall.h"

VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size) : _vboID(0) {
	GLCall(glGenBuffers(1, &_vboID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->_vboID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	Clean();
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->_vboID));
}

void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::Clean() {
	Unbind();
	GLCall(glDeleteBuffers(1, &_vboID));
}
