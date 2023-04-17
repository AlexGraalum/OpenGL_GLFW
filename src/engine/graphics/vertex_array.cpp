#include "vertex_array.h"

VertexArray::VertexArray() {
     glGenVertexArrays(1, &ID);
}

void VertexArray::LinkAttribute(VertexBuffer vertexBuffer, GLuint layout, GLuint compCount, GLenum type, GLsizeiptr stride, void* offset) {
     vertexBuffer.Bind();
     glVertexAttribPointer(layout, compCount, type, GL_FALSE, (GLsizei)stride, offset);
     glEnableVertexAttribArray(layout);
     vertexBuffer.Unbind();
}

void VertexArray::Bind() {
     glBindVertexArray(ID);
}

void VertexArray::Unbind() {
     glBindVertexArray(0);
}

void VertexArray::Delete() {
     glDeleteVertexArrays(1, &ID);
}