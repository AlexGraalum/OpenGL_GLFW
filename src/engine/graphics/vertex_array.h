#pragma once

#include <glad\glad.h>
#include "vertex_buffer.h"

class VertexArray {
public:
     GLuint ID;
     VertexArray();

     void LinkAttribute(VertexBuffer vertexBuffer, GLuint layout, GLuint componentsCount, GLenum type, GLsizeiptr stride, void* offset);
     void Bind();
     void Unbind();
     void Delete();
};