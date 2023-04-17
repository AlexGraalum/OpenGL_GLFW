#include "object.h"

Object::Object(Mesh* mesh, Shader* shader, Texture* texture, glm::vec3 position, glm::vec4 rotation, glm::vec3 scale){
     this->objectMesh = mesh;
     this->objectShader = shader;
     this->objectTexture = texture;

     SetPosition(position);
     SetRotation(rotation);
     SetScale(scale);
}

Object::~Object() {
     this->objectMesh = nullptr;
     this->objectShader = nullptr;
     this->objectTexture = nullptr;
}

std::shared_ptr<Object> Object::CreateObject(Mesh* mesh, Shader* shader, Texture* texture, glm::vec3 position, glm::vec4 rotation, glm::vec3 scale){
     return std::shared_ptr<Object>(new Object(mesh, shader, texture, position, rotation, scale));
}

void Object::Draw3D(std::shared_ptr<Camera> camera, glm::vec3 lightPos) {
     glEnable(GL_DEPTH_TEST);

     glm::mat4 projection = camera->GetPerspective();

     glm::mat4 view = camera->GetView();

     glm::mat4 model(1.0f);
     model = glm::translate(model, position);
     model = glm::rotate(model, glm::radians(rotation[3]), glm::vec3(rotation[0], rotation[1], rotation[2]));
     model = glm::scale(model, scale);

     glm::vec3 camPos = camera->GetPosition();

     if (this->objectTexture != nullptr) this->objectTexture->Bind(0);

     if (this->objectShader != nullptr) {
          this->objectShader->use();
          this->objectShader->setMat4("view", view);
          this->objectShader->setMat4("projection", projection);
          this->objectShader->setMat4("model", model);
          this->objectShader->setVec3("viewPos", camPos);
          this->objectShader->setVec3("lightPos", lightPos);
     }

     this->objectMesh->DrawMesh();

     if (this->objectTexture != nullptr) this->objectTexture->Unbind();
}

void Object::Draw2D() {
     glDisable(GL_DEPTH_TEST);

     glm::mat4 projection(1.0f);
     glm::mat4 view(1.0f);

     glm::mat4 model(1.0f);
     model = glm::translate(model, position);
     model = glm::rotate(model, glm::radians(rotation[3]), glm::vec3(rotation[0], rotation[1], rotation[2]));
     model = glm::scale(model, scale);

     if (this->objectTexture != nullptr) this->objectTexture->Bind(0);

     if (this->objectShader != nullptr) {
          this->objectShader->use();
          this->objectShader->setMat4("view", view);
          this->objectShader->setMat4("projection", projection);
          this->objectShader->setMat4("model", model);
     }

     this->objectMesh->DrawMesh();

     if (this->objectTexture != nullptr) this->objectTexture->Unbind();
}