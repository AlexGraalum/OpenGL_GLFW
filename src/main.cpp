//Includes
#include "engine\graphics\window.h"
#include "engine\graphics\camera.h"

#include "engine\object.h"
#include "engine\file_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//DEFINES
#define MINSPIN 1.0f
#define MAXSPIN 100.0f

//PROTOTYPES
void framebuffer_size_callback(GLFWwindow*, int, int);
void keyCallback(GLFWwindow*, int, int, int, int);

void processInput();

void Animate();

//GLOBALS
std::shared_ptr<Window> windowPtr;
std::shared_ptr<Camera> camera;

FileManager<Mesh>* meshManager;
FileManager<Shader>* shaderManager;
FileManager<Texture>* textureManager;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool spin = false;
float angle = 0.0f;
float angleIncrement = 10.0f;

glm::vec3 lightPos;

//CONSTANT GLOBALS
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

////3D Object Transforms
//Cube
const glm::vec3 cubePos[] = {
     glm::vec3(0.0f, 0.0f, 0.0f),
};
const glm::vec4 cubeRot[] = {
               //Vector          //Radians
     glm::vec4(1.0f, 0.3f, 0.5f, angle),
};
const glm::vec3 cubeScale[] = {
     glm::vec3(1.0f),
};
//Pyramid
const glm::vec3 pyramidPos[] = {
     glm::vec3(0.0f, 1.0f, 0.0f),
};
const glm::vec4 pyramidRot[] = {
               //Vector          //Radians
     glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
};
const glm::vec3 pyramidScale[] = {
     glm::vec3(1.0f, 1.0f, 1.0f),
};

////2D Objects
//Square
std::vector<Vertex> squareVertices = {
     //               position                       normal                       texture
     Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
     Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
     Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
     Vertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
};
std::vector<unsigned int> squareIndices = {
     0, 1, 3,
     1, 2, 3,
};
const glm::vec3 squarePos[] = {
     glm::vec3(0.5f, 0.5f, 0.0f),
     glm::vec3(-0.5f, -0.5f, 0.0f),
};
const float squareRot[] = {
     0.0f,
     0.0f,
};
const glm::vec2 squareScale[] = {
     glm::vec2(0.1f),
     glm::vec2(0.05f),
};
//Triangle
GLfloat triangleVertices[] = {
     //vertices               colors              texture
     -0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
      0.0f,  0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   0.5f, 1.0f,
      0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
};
GLuint triangleIndices[] = {
     0, 1, 2
};
const glm::vec3 trianglePos[] = {
     glm::vec3(-0.3f, 0.65f, 0.0f),
};
const float triangleRot[] = {
     15.0f,
};
const glm::vec2 triangleScale[] = {
     glm::vec2(0.5f),
};

//FUNCTIONS
int main(int argc, char** argv) {

     //Create Window
     windowPtr = Window::CreateWindow("Learn OpenGL", SCR_WIDTH, SCR_HEIGHT, framebuffer_size_callback, keyCallback);

     //Create Camera
     camera = Camera::CreateCamera(glm::vec3(0.0f, 0.0f, 6.0f),
                                   glm::vec2(SCR_WIDTH, SCR_HEIGHT),
                                   45.0f,
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, 1.0f, 0.0f),
                                   2.0f);

     camera->MoveBack(5.0f);

     //Initialize Managers
     meshManager = new FileManager<Mesh>();
     shaderManager = new FileManager<Shader>();
     textureManager = new FileManager<Texture>();

     //Load Meshes
     meshManager->LoadFile("resources/models/cube.obj");
     meshManager->LoadFile("resources/models/ship.obj");
     meshManager->LoadFile("resources/models/pyramid.obj");
     meshManager->LoadFile("resources/models/square.obj");

     //Load Shaders
     shaderManager->LoadFile("resources/shaders/litCube.vs", "resources/shaders/litCube.fs");
     shaderManager->LoadFile("resources/shaders/cube.vs", "resources/shaders/cube.fs");
     shaderManager->LoadFile("resources/shaders/square.vs", "resources/shaders/square.fs");

     //Load Textures
     textureManager->LoadFile("resources/textures/cat.jpg");
     textureManager->LoadFile("resources/textures/catCube.jpg");
     textureManager->LoadFile("resources/textures/catCubeLabeled.png");

     //Create Objects
     auto unlitCube = Object::CreateObject(  meshManager->GetValue("cube"),     shaderManager->GetValue("cube"),        nullptr,                           glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.1f));
     auto litCube =   Object::CreateObject(  meshManager->GetValue("cube"),     shaderManager->GetValue("litCube"),     textureManager->GetValue("catCubeLabeled"),   glm::vec3(1.0f, 0.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.5f));
     auto pyramid =   Object::CreateObject(  meshManager->GetValue("pyramid"),  shaderManager->GetValue("litCube"),     textureManager->GetValue("cat"),   glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.25f));
     auto ship =      Object::CreateObject(  meshManager->GetValue("ship"),     shaderManager->GetValue("litCube"),     nullptr,                           glm::vec3(0.0f, 1.0f, 0.0f),  glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.25f));

     auto square = Object::CreateObject(meshManager->GetValue("square"), shaderManager->GetValue("square"), textureManager->GetValue("cat"), glm::vec3(0.4f, 0.4f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.2f));

     //std::shared_ptr<Object> object = Object::CreateObject("resources/models/cube.obj", "resources/shaders/litCube.vs", "resources/shaders/litCube.fs", "resources/textures/cat.jpg", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec3(1.0f));
     //std::shared_ptr<Object> object = Object::CreateObject(meshManager->GetValue("cube"), shaderManager->GetValue("litCube"), textureManager->GetValue("cat"), glm::vec3(0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
     //Object *object = new Object(meshManager->GetValue("cube"), shaderManager->GetValue("litCube"), textureManager->GetValue("cat"), glm::vec3(0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
     //std::shared_ptr<Object> object = Object::CreateObject(meshManager->GetValue("cube"), nullptr, nullptr, glm::vec3(0.0f), glm::vec4(0.0f), glm::vec3(1.0f));

     ////Load and Create Texture
     //auto texture = Texture::CreateTexture("resources/textures/cat.jpg");
     //auto catCubeTex = Texture::CreateTexture("resources/textures/catCube.png");
     //
     ////Build Shader
     //auto unLitCubeShader = Shader::Create("resources/shaders/cube.vs", "resources/shaders/cube.fs");
     //auto litCubeShader = Shader::Create("resources/shaders/litCube.vs", "resources/shaders/litCube.fs");
     //auto squareShader = Shader::Create("resources/shaders/square.vs", "resources/shaders/square.fs");
     //
     ////Prepare Meshes
     //auto squareMesh = Mesh::CreateMesh(squareVertices, squareIndices);
     //auto cubeMesh = Mesh::CreateMesh("resources/models/cube.obj");
     //auto pyramidMesh = Mesh::CreateMesh("resources/models/testPyramid.obj");
     //
     //auto shipMesh = Mesh::CreateMesh("resources/models/ship.obj");

     //lightPos = glm::vec3(-1.0f, 2.0f, 0.0f);

     //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
     while (!windowPtr->ShouldClose()) {
          glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
          float currentFrame = static_cast<float>(glfwGetTime());
          deltaTime = currentFrame - lastFrame;
          lastFrame = currentFrame;

          Animate();

          //Process Inputs
          processInput();

          //Clear Screen
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          ////Render 3D Scene
          unlitCube->Draw3D(camera, glm::vec3(0.0f));

          litCube->Draw3D(camera, unlitCube->GetPosition());
          pyramid->Draw3D(camera, unlitCube->GetPosition());
          ship->Draw3D(camera, unlitCube->GetPosition());

          //square->Draw3D(camera, glm::vec3(0.0f));

          //object->Draw3D(camera, lightPos);
          //cubeMesh->Draw3D(glm::vec3(0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.5f), camera, { unLitCubeShader }, { texture }, {});
          //cubeMesh->Draw3D(lightPos, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.1f), camera, { unLitCubeShader }, { NULL }, {});
          //
          //cubeMesh->Draw3D(   glm::vec3( 1.0f,  1.0f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, angle), glm::vec3(0.5f), camera, { litCubeShader }, { NULL },       lightPos);
          //pyramidMesh->Draw3D(glm::vec3( 1.0f, -1.0f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, angle), glm::vec3(0.5f), camera, { litCubeShader }, { texture },    lightPos);
          //cubeMesh->Draw3D(   glm::vec3(-1.0f, -1.0f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, angle), glm::vec3(0.5f), camera, { litCubeShader }, { catCubeTex }, lightPos);
          //
          //shipMesh->Draw3D(   glm::vec3(-1.0f,  1.0f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, angle), glm::vec3(1.0f), camera, { litCubeShader }, { NULL },       lightPos);

          //Render 2D Scene
          //pyramid->Draw2D();
          square->Draw2D();
          //squareMesh->Draw2D(glm::vec2(200.0f, 0.0f), 0.0f, glm::vec2(0.5f), camera, { squareShader }, { texture });
          //for (unsigned int i = 0; i < (sizeof(squarePos) / sizeof(squarePos[0])); i++) {
          //     squareMesh->Draw2D(squarePos[i], squareRot[i], squareScale[i], camera, {squareShader}, {texture});
          //}

          //cubeMesh->Draw3DIn2D(glm::vec2(200.0f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, angle), glm::vec3(0.1f), camera, { unLitCubeShader }, { catCubeTex }, {});

          //Poll Events and Swap Buffers
          windowPtr->Flush();
     }

     return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
     glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
     if (action == GLFW_PRESS) {
          switch (key) {
          //Spin Toggle
          case GLFW_KEY_R:
               spin = !spin;
               break;

          //Spin Speed Increase
          case GLFW_KEY_EQUAL:
               if (angleIncrement < MAXSPIN) {
                    if (mods == GLFW_MOD_SHIFT && MAXSPIN - angleIncrement >= 5.0f) {
                         angleIncrement += 5.0f;
                    } else {
                         angleIncrement += 1.0f;
                    }
                    windowPtr->UpdateTitle("SpinSpeed = " + std::to_string(angleIncrement));
               }
               break;
          //Spin Speed Decrease
          case GLFW_KEY_MINUS:
               if (angleIncrement > MINSPIN) {
                    if (mods == GLFW_MOD_SHIFT && angleIncrement - MINSPIN >= 5.0f) {
                         angleIncrement -= 5.0f;
                    } else {
                         angleIncrement -= 1.0f;
                    }
                    windowPtr->UpdateTitle("SpinSpeed = " + std::to_string(angleIncrement));
               }
               break;
          }
     }
}

void processInput() {
     //Close Window
     if (windowPtr->ProcessInput(GLFW_KEY_ESCAPE)) {
          windowPtr->Close();
     }

     //Camera Control
     if (windowPtr->ProcessInput(GLFW_KEY_W)) camera->MoveFront(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_S)) camera->MoveBack(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_A)) camera->MoveLeft(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_D)) camera->MoveRight(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_SPACE)) camera->MoveUp(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_LEFT_CONTROL)) camera->MoveDown(deltaTime);

     //Light Source Control
     if (windowPtr->ProcessInput(GLFW_KEY_J)) { lightPos += (deltaTime * 10.0f) * glm::vec3(1.0f, 0.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_L)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(1.0f, 0.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_K)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(0.0f, 0.0f, 1.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_I)) { lightPos += (deltaTime * 10.0f) * glm::vec3(0.0f, 0.0f, 1.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_U)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(0.0f, 1.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_O)) { lightPos += (deltaTime * 10.0f) * glm::vec3(0.0f, 1.0f, 0.0f); }
}

void Animate() {
     if(spin) angle += (deltaTime * angleIncrement);
}