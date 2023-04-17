# pragma once

#include "graphics\mesh.h"
#include "graphics\texture.h"
#include "graphics\shader.h"


class Object {
private:
     Mesh* objectMesh;
     Texture* objectTexture;
     Shader* objectShader;

     glm::vec3 position = glm::vec3(0.0f);
     glm::vec4 rotation;
     glm::vec3 scale;
public:
     Object(Mesh* mesh, Shader* shader, Texture* texture, glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);

     ~Object();

     static std::shared_ptr<Object> CreateObject(Mesh* mesh, Shader* shader, Texture* texture, glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);

     void SetPosition(glm::vec3 position) { this->position = position; }
     glm::vec3 GetPosition() { return this->position; }

     void SetRotation(glm::vec4 rotation) { this->rotation = rotation; }
     glm::vec4 GetRotation() { return this->rotation; }

     void SetScale(glm::vec3 scale) { this->scale = scale; }
     glm::vec3 GetScale() { return this->scale; }

     void Draw3D(std::shared_ptr<Camera> camera, glm::vec3 lightPos);
     void Draw2D();
};