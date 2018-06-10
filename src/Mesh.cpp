#include "../include/Mesh.h"

Mesh::Mesh(const void* data, GLsizeiptr size, unsigned int count) :
	_vao(),
	_vbo(data, size) 
{
	VertexBufferLayout layout = VertexBufferLayout();
	layout.Push<float>(2);
	layout.Push<float>(2);
	_count = count; 	
	_vao.AddBuffer(&_vbo, &layout);	
}	

Mesh::~Mesh() {
	Clean();
}

void Mesh::Bind() {
	_vao.Bind();
}
	
void Mesh::Unbind() {
	_vao.Unbind();
}

void Mesh::Clean() {
	_vao.Clean();
	_vbo.Clean();
}

