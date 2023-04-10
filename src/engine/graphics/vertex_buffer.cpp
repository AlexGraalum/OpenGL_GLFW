#include "vertex_buffer.h"

VertexBuffer::VertexBuffer(Vertex* vertices, GLsizeiptr size) {
     glGenBuffers(1, &ID);
     glBindBuffer(GL_ARRAY_BUFFER, ID);
     glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() {
     glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::Unbind() {
     glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete() {
     glDeleteBuffers(1, &ID);
}