#ifndef MESH_H
#define MESH_H

#include "VertexArray.h"


// A mesh store vertices in the form { x, y, u, v }, where each
// vertex is 32 a bit floating point number. It holds its own vertex
// buffer and vertex array objects.
// Note however that the mesh does not hold information about textures,
// so it will use whatever texture is bound to the context at the moment. 
// The data layout is defined internally, so it's not possible to
// use other data representations. It's designed for 2D drawing.

class Mesh {
public:
	Mesh(const void* data, GLsizeiptr size, unsigned int count);
	~Mesh();
	
	unsigned int count() const { return _count; }

	void Bind();
	void Unbind();

	void Clean();
	
private:
	VertexArray _vao;
	VertexBuffer _vbo;
	unsigned int _count;

	//Disables copy constructor
	//This destructor calls glDelete, so if the object used for copying is deleted or
	//goes out of scope, this object becomes unusable and will lead to a runtime error
	Mesh(const Mesh&); 
};

#endif
