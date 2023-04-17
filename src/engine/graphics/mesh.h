#pragma once

#include <string>
#include <vector>
#include <memory>

#include <sstream>
#include <fstream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <glad\glad.h>

#include "vertex.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "element_buffer.h"

#include "camera.h"

class Mesh {
private:
     VertexArray* vertexArray;
     VertexBuffer* vertexBuffer;
     ElementBuffer* elementBuffer;

     std::vector<Vertex> vertices;
     std::vector<unsigned int> indices;

     void LoadFile(std::string);
     void PrepareMesh();
public:
     Mesh(std::vector<Vertex>, std::vector<unsigned int>);
     Mesh(std::string filePath);

     ~Mesh();

     static std::shared_ptr<Mesh> Create(std::vector<Vertex>, std::vector<unsigned int>);
     static std::shared_ptr<Mesh> Create(std::string filePath);

     void DrawMesh();
};