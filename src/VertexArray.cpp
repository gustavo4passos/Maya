#include "../include/VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &_vaoID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &_vaoID);
}

void VertexArray::Bind() const {
    glBindVertexArray(_vaoID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(VertexBuffer& vbo, VertexBufferLayout& layout) {
    Bind();
    vbo.Bind();

    unsigned int offset = 0;

    for(int i = 0; i < layout.elements().size(); i++) {
        VertexBufferLayoutElement element = layout.elements()[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.stride(), (const void*)offset);
        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    } 
}
