#include "element_buffer.h"

ElementBuffer::ElementBuffer(GLuint* indices, GLsizeiptr size) {
     glGenBuffers(1, &ID);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void ElementBuffer::Bind() {
     glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void ElementBuffer::Unbind() {
     glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ElementBuffer::Delete() {
     glDeleteBuffers(1, &ID);
}