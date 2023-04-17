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

//#include "shader.h"
//#include "texture.h"

#include "camera.h"

class Mesh {
private:
     VertexArray* vertexArray;
     VertexBuffer* vertexBuffer;
     ElementBuffer* elementBuffer;

     std::vector<Vertex> vertices;
     std::vector<unsigned int> indices;

     //glm::vec3 positionOffset;
     //glm::vec4 rotation;
     //glm::vec3 scale;

     void LoadFile(std::string);
     void PrepareMesh();
public:
     Mesh(std::vector<Vertex>, std::vector<unsigned int>);
     Mesh(std::string filePath);

     ~Mesh();

     static std::shared_ptr<Mesh> Create(std::vector<Vertex>, std::vector<unsigned int>);
     static std::shared_ptr<Mesh> Create(std::string filePath);

     //void SetPositionOffset(glm::vec3 offset) { this->positionOffset = offset; }
     //glm::vec3 GetPositionOffset() { return this->positionOffset; }
     //
     //void SetRotation(glm::vec4 rotation) { this->rotation = rotation; }
     //glm::vec4 GetRotation() { return this->rotation; }
     //
     //void SetScale(glm::vec3 scale) { this->scale = scale; }
     //glm::vec3 GetScale() { return this->scale; }

     //void Draw3D(    glm::vec3 translation, glm::vec4 rotation, glm::vec3 scale, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Shader>> shaders, std::vector<std::shared_ptr<Texture>> textures, glm::vec3 lightPos);
     //void Draw3DIn2D(glm::vec2 translation, glm::vec4 rotation, glm::vec3 scale, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Shader>> shaders, std::vector<std::shared_ptr<Texture>> textures, glm::vec3 lightPos);
     //void Draw2D(    glm::vec2 translation, float rotation,     glm::vec2 scale, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Shader>> shaders, std::vector<std::shared_ptr<Texture>> textures);

     void DrawMesh();
};