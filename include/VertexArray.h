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

    void AddBuffer(VertexBuffer* vbo, VertexBufferLayout* layout); 
    void Bind() const;
    void Unbind() const;

	void Clean();

private:
    GLuint _vaoID;

	//Disables copy constructor
	//This destructor calls glDelete, so if the object used for copying is deleted or
	//goes out of scope, this object becomes unusable and will lead to a runtime error
	VertexArray(const VertexArray&);
};

#endif
