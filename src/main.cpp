//#include <glad\glad.h>
//#include <GLFW\glfw3.h>

#define WINDOWHEADER

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine\graphics\window.h"

#include "engine\graphics\vertex_array.h"
#include "engine\graphics\vertex_buffer.h"
#include "engine\graphics\element_buffer.h"

#include "engine\graphics\texture.h"
#include "engine\graphics\mesh.h"
#include "engine\graphics\shader.h"

#include "engine\graphics\camera.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <string>

//PROTOTYPES
void framebuffer_size_callback(GLFWwindow*, int, int);
void keyCallback(GLFWwindow*, int, int, int, int);

void processInput();

void Animate();

//GLOBALS
#ifdef WINDOWHEADER
std::shared_ptr<Window> windowPtr;
#else
GLFWwindow* windowPtr;
#endif
std::shared_ptr<Camera> camera;

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
     //position                                      normal                       texture
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
     glm::vec2(0.5f),
     glm::vec2(0.25f),
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
#ifdef WINDOWHEADER
     windowPtr = Window::CreateWindow("Learn OpenGL", SCR_WIDTH, SCR_HEIGHT, framebuffer_size_callback, keyCallback);
#else
     glfwInit();
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
     glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
     
     window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
     if (window == NULL) {
          fprintf(stderr, "Failed to create GLFW window\n");
          glfwTerminate();
          return -1;
     }
     
     glfwMakeContextCurrent(window);
     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
     glfwSetKeyCallback(window, keyCallback);
     
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
          fprintf(stderr, "Failed to initialize GLAD\n");
          return -1;
     }
#endif



     //Create Camera
     camera = Camera::CreateCamera(glm::vec3(0.0f, 0.0f, 6.0f),
                                        45.0f,
                                        glm::vec3(0.0f, 0.0f, 0.0f),
                                        glm::vec3(0.0f, 1.0f, 0.0f),
                                        2.0f);

     camera->MoveBack(5.0f);

     //Load and Create Texture
     auto texture = Texture::CreateTexture("resources/textures/cat.jpg");
     auto catCubeTex = Texture::CreateTexture("resources/textures/catCube.png");

     //Build Shader
     auto unLitCubeShader = Shader::Create("resources/shaders/cube.vs", "resources/shaders/cube.fs");
     auto litCubeShader = Shader::Create("resources/shaders/litCube.vs", "resources/shaders/litCube.fs");
     auto squareShader = Shader::Create("resources/shaders/square.vs", "resources/shaders/square.fs");

     //Prepare Meshes
     auto squareMesh = Mesh::CreateMesh(squareVertices, squareIndices);
     auto cubeMesh = Mesh::CreateMesh("resources/models/cube.obj");
     auto pyramidMesh = Mesh::CreateMesh("resources/models/testPyramid.obj");

     auto shipMesh = Mesh::CreateMesh("resources/models/ship.obj");

     lightPos = glm::vec3(0.0f);

     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
#ifdef WINDOWHEADER
     while (!windowPtr->ShouldClose()) {
#else
     while(!glfwWindowShouldClose(window)){
#endif
          float currentFrame = static_cast<float>(glfwGetTime());
          deltaTime = currentFrame - lastFrame;
          lastFrame = currentFrame;

          Animate();

          //Process Inputs
          processInput();

          //Clear Screen
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          //Render 3D Scene
          cubeMesh->Draw3D(lightPos, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.1f), camera, { unLitCubeShader }, { NULL }, {});

          cubeMesh->Draw3D(   glm::vec3( 1.0f,  1.0f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, angle), glm::vec3(0.5f), camera, { litCubeShader }, { NULL }, lightPos);
          pyramidMesh->Draw3D(glm::vec3( 1.0f, -1.0f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, angle), glm::vec3(0.5f), camera, { litCubeShader }, { texture }, lightPos);
          cubeMesh->Draw3D(   glm::vec3(-1.0f, -1.0f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, angle), glm::vec3(0.5f), camera, { litCubeShader }, { catCubeTex }, lightPos);

          shipMesh->Draw3D(   glm::vec3(-1.0f,  1.0f,  0.0f), glm::vec4(0.0f, 1.0f, 0.0f, angle), glm::vec3(1.0f), camera, {litCubeShader}, { NULL }, lightPos);

          //Render 2D Scene
          for (unsigned int i = 0; i < (sizeof(squarePos) / sizeof(squarePos[0])); i++) {
               squareMesh->Draw2D(squarePos[i], squareRot[i], squareScale[i], {squareShader}, {texture});
          }
          
          cubeMesh->Draw3DIn2D(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, angle), glm::vec3(0.1f), { unLitCubeShader }, { catCubeTex }, {});

          //Poll Events and Swap Buffers
#ifdef WINDOWHEADER
          windowPtr->Flush();
#else
          glfwSwapBuffers(window);
          glfwPollEvents();
#endif
     }

#ifndef WINDOWHEADER
     glfwDestroyWindow(window);
     glfwTerminate();
#endif
     return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
     glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
     if (key == GLFW_KEY_R && action == GLFW_PRESS) spin = !spin;
     if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS && angleIncrement < 15.0f) angleIncrement += 1.0f;
     if (key == GLFW_KEY_MINUS && action == GLFW_PRESS && angleIncrement > 5.0f) angleIncrement -= 1.0f;
}

void processInput() {
#ifdef WINDOWHEADER
     if (windowPtr->ProcessInput(GLFW_KEY_ESCAPE)) {
          windowPtr->Close();
     }
     if (windowPtr->ProcessInput(GLFW_KEY_W)) camera->MoveFront(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_S)) camera->MoveBack(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_A)) camera->MoveLeft(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_D)) camera->MoveRight(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_SPACE)) camera->MoveUp(deltaTime);
     if (windowPtr->ProcessInput(GLFW_KEY_LEFT_CONTROL)) camera->MoveDown(deltaTime);

     if (windowPtr->ProcessInput(GLFW_KEY_J)) { lightPos += (deltaTime * 10.0f) * glm::vec3(1.0f, 0.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_L)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(1.0f, 0.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_K)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(0.0f, 0.0f, 1.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_I)) { lightPos += (deltaTime * 10.0f) * glm::vec3(0.0f, 0.0f, 1.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_U)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(0.0f, 1.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_O)) { lightPos += (deltaTime * 10.0f) * glm::vec3(0.0f, 1.0f, 0.0f); }
#else
     if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
          glfwSetWindowShouldClose(windowPtr, true);
     }

     if (glfwGetKey(windowPtr, GLFW_KEY_W) == GLFW_PRESS) camera->MoveFront(deltaTime);
     if (glfwGetKey(windowPtr, GLFW_KEY_S) == GLFW_PRESS) camera->MoveBack(deltaTime);
     if (glfwGetKey(windowPtr, GLFW_KEY_A) == GLFW_PRESS) camera->MoveLeft(deltaTime);
     if (glfwGetKey(windowPtr, GLFW_KEY_D) == GLFW_PRESS) camera->MoveRight(deltaTime);
     if (glfwGetKey(windowPtr, GLFW_KEY_SPACE) == GLFW_PRESS) camera->MoveUp(deltaTime);
     if (glfwGetKey(windowPtr, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera->MoveDown(deltaTime);

     if (glfwGetKey(windowPtr, GLFW_KEY_J) == GLFW_PRESS)   { lightPos += (deltaTime * 10.0f) * glm::vec3( 1.0f, 0.0f, 0.0f); }
     if (glfwGetKey(windowPtr, GLFW_KEY_L) == GLFW_PRESS)   { lightPos -= (deltaTime * 10.0f) * glm::vec3( 1.0f, 0.0f, 0.0f); }
     if (glfwGetKey(windowPtr, GLFW_KEY_K) == GLFW_PRESS)   { lightPos -= (deltaTime * 10.0f) * glm::vec3( 0.0f, 0.0f, 1.0f); }
     if (glfwGetKey(windowPtr, GLFW_KEY_I) == GLFW_PRESS)   { lightPos += (deltaTime * 10.0f) * glm::vec3( 0.0f, 0.0f, 1.0f); }
     if (glfwGetKey(windowPtr, GLFW_KEY_U) == GLFW_PRESS)   { lightPos -= (deltaTime * 10.0f) * glm::vec3( 0.0f, 1.0f, 0.0f); }
     if (glfwGetKey(windowPtr, GLFW_KEY_O) == GLFW_PRESS)   { lightPos += (deltaTime * 10.0f) * glm::vec3( 0.0f, 1.0f, 0.0f); }
#endif
}

void Animate() {
     if(spin) angle += (deltaTime * angleIncrement);
}