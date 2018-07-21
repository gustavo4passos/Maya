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
	
	void Clean();

private:
	GLuint _vboID;

	//Disables copy constructor
	//This destructor calls glDelete, so if the object used for copying is deleted or
	//goes out of scope, this object becomes unusable and will lead to a runtime error
	VertexBuffer(const VertexBuffer& v); 
};

#endif
