#pragma once

#include <glad\glad.h>

class ElementBuffer {
public:
     GLuint ID;
     ElementBuffer(GLuint*, GLsizeiptr);

     void Bind();
     void Unbind();
     void Delete();
};