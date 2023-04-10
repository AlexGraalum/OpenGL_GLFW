#pragma once

#include <glad\glad.h>

#include "vertex.h"

class VertexBuffer {
public:
     GLuint ID;
     VertexBuffer(Vertex* , GLsizeiptr );

     void Bind();
     void Unbind();
     void Delete();
};