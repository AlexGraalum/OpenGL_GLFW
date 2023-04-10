#include "camera.h"

Camera::Camera(glm::vec3 position, float fov, glm::vec3 target, glm::vec3 up, float speed){
     this->cameraPos = glm::vec3(position);
     this->cameraFov = fov;
     this->cameraTarget = glm::vec3(target);

     cameraFront = glm::normalize(this->cameraPos - this->cameraTarget);
     glm::vec3 right = glm::normalize(glm::cross(up, cameraFront));
     this->cameraUp = glm::cross(cameraFront, right);

     this->cameraSpeed = float(speed);

     this->UpdateViewProjection();
}

Camera::~Camera() {

}

std::shared_ptr<Camera> Camera::CreateCamera(glm::vec3 position, float fov, glm::vec3 target, glm::vec3 up, float speed) {
                                               //Position                   fov    speed
     return std::shared_ptr<Camera>(new Camera(position, fov, target, up, speed));
}

void Camera::UpdateViewProjection() {
     this->view = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
     this->projection = glm::perspective(glm::radians(this->cameraFov), (float)800/600, 0.1f, 100.0f);
}

void Camera::MoveFront(float dt) {
     this->cameraPos += (this->cameraSpeed * dt) * this->cameraFront;
     this->UpdateViewProjection();
}
void Camera::MoveBack(float dt) {
     this->cameraPos -= (this->cameraSpeed * dt) * this->cameraFront;
     this->UpdateViewProjection();
}
void Camera::MoveRight(float dt) {
     this->cameraPos += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * (this->cameraSpeed * dt);
     this->UpdateViewProjection();
}
void Camera::MoveLeft(float dt) {
     this->cameraPos -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * (this->cameraSpeed * dt);
     this->UpdateViewProjection();
}
void Camera::MoveUp(float dt) {
     this->cameraPos += (this->cameraSpeed * dt) * this->cameraUp;
     this->UpdateViewProjection();
}
void Camera::MoveDown(float dt) {
     this->cameraPos -= (this->cameraSpeed * dt) * this->cameraUp;
     this->UpdateViewProjection();
}