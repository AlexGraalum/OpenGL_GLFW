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

#include "shader.h"
#include "texture.h"

#include "camera.h"

class Mesh {
private:
     VertexArray* vertexArray;
     VertexBuffer* vertexBuffer;
     ElementBuffer* elementBuffer;

     std::vector<Vertex> vertices;
     std::vector<unsigned int> indices;

     glm::vec3 positionOffset;
     glm::vec4 rotation;
     glm::vec3 scale;

     Mesh(std::vector<Vertex>, std::vector<unsigned int>);
     Mesh(std::string filePath);

     void LoadFile(std::string);
     void PrepareMesh();
public:
     ~Mesh();

     static std::shared_ptr<Mesh> CreateMesh(std::vector<Vertex>, std::vector<unsigned int>);
     static std::shared_ptr<Mesh> CreateMesh(std::string filePath);

     void SetPositionOffset(glm::vec3 offset) { this->positionOffset = offset; }
     glm::vec3 GetPositionOffset() { return this->positionOffset; }

     void SetRotation(glm::vec4 rotation) { this->rotation = rotation; }
     glm::vec4 GetRotation() { return this->rotation; }

     void SetScale(glm::vec3 scale) { this->scale = scale; }
     glm::vec3 GetScale() { return this->scale; }

     void Draw3D(glm::vec3, glm::vec4, glm::vec3, std::shared_ptr<Camera>, std::vector<std::shared_ptr<Shader>>, std::vector<std::shared_ptr<Texture>>, glm::vec3);
     void Draw3DIn2D(glm::vec3, glm::vec4, glm::vec3, std::vector<std::shared_ptr<Shader>>, std::vector<std::shared_ptr<Texture>>, glm::vec3);
     void Draw2D(glm::vec3, float, glm::vec2, std::vector<std::shared_ptr<Shader>>, std::vector<std::shared_ptr<Texture>>);
};