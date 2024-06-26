#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
     glm::vec3 cameraPos;
     float cameraFov;
     glm::vec3 cameraTarget;

     glm::vec3 cameraFront;
     glm::vec3 cameraUp;

     glm::mat4 view;

     glm::vec2 screenSize;

     glm::mat4 perspective;
     glm::mat4 orthographic;

     float cameraSpeed;

     Camera(glm::vec3 position, glm::vec2 screen, float fov, float speed);
     Camera(glm::vec3 position, glm::vec2 screen, float fov, glm::vec3 target, glm::vec3 up, float speed);
public:
     ~Camera();

     static std::shared_ptr<Camera> CreateCamera(glm::vec3 position, glm::vec2 screen, float fov, glm::vec3 target, glm::vec3 up, float speed);

     void SetScreenSize(glm::vec2 screen) { this->screenSize = screen; }
     glm::vec2 GetScreenSize() { return this->screenSize; }

     void SetPosition(glm::vec3 position) { this->cameraPos = position; }
     glm::vec3 GetPosition() { return this->cameraPos; }

     void SetFov(float fov) { this->cameraFov = fov; }
     float GetFov() { return this->cameraFov; }

     void SetTarget(glm::vec3 target) { this->cameraTarget = target; }
     glm::vec3 GetTarget() { return this->cameraTarget; }

     void SetFront(glm::vec3 front) { this->cameraFront = front; }
     glm::vec3 GetFront() { return this->cameraFront; }

     void SetUp(glm::vec3 up) { this->cameraUp = up; }
     glm::vec3 GetUp() { return this->cameraUp; }

     void UpdateViewProjection();
     glm::mat4 GetView() { return this->view; }
     glm::mat4 GetPerspective() { return this->perspective; }
     glm::mat4 GetOrthographic() { return this->orthographic; }

     void SetSpeed(float speed) { this->cameraSpeed = speed; }
     float GetSpeed() { return this->cameraSpeed; }

     void MoveFront(float dt);
     void MoveBack(float dt);
     void MoveRight(float dt);
     void MoveLeft(float dt);
     void MoveUp(float dt);
     void MoveDown(float dt);
};