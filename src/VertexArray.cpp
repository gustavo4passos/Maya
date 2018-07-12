#include "../include/VertexArray.h"

#include "../include/GLCall.h"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &_vaoID));
}

VertexArray::~VertexArray() {
	Clean();
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(_vaoID));
}

void VertexArray::Unbind() const {
    GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(VertexBuffer* vbo, VertexBufferLayout* layout) {
    Bind();
    vbo->Bind();

    unsigned int offset = 0;

    for(unsigned int i = 0; i < layout->elements().size(); i++) {
        VertexBufferLayoutElement element = layout->elements()[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout->stride(), (const void*)offset));
        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    } 
}

void VertexArray::Clean() {
	Unbind();
	GLCall(glDeleteVertexArrays(1, &_vaoID));
}
