//Includes
#include "engine\graphics\window.h"
#include "engine\graphics\camera.h"

#include "engine\object.h"

#include "engine\file_manager.h"
#include "engine\object_manager.h"

//PROTOTYPES
void framebuffer_size_callback(GLFWwindow*, int, int);
void keyCallback(GLFWwindow*, int, int, int, int);

void processInput();
void Animate();

//GLOBALS
std::shared_ptr<Window> windowPtr;
std::shared_ptr<Camera> camera;

std::shared_ptr<FileManager<Mesh>> meshManager;
std::shared_ptr<FileManager<Shader>> shaderManager;
std::shared_ptr<FileManager<Texture>> textureManager;

std::shared_ptr<ObjectManager> worldObjectManager;
std::shared_ptr<ObjectManager> screenObjectManager;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool spin = false;
float angle = 0.0f;
float angleIncrement = 10.0f;

glm::vec3 lightPos = glm::vec3(0.0f);

//CONSTANT GLOBALS
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const unsigned int MIN_SPIN = 10;
const unsigned int MAX_SPIN = 100;

//FUNCTIONS
int main(int argc, char** argv) {

     //Create Window
     windowPtr = Window::CreateWindow("Render System", SCR_WIDTH, SCR_HEIGHT, framebuffer_size_callback, keyCallback, false);

     //Create Camera
     camera = Camera::CreateCamera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec2(SCR_WIDTH, SCR_HEIGHT), 45.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.0f);
     camera->MoveBack(5.0f);

     //Initialize Managers
     meshManager = FileManager<Mesh>::Create();
     shaderManager = FileManager<Shader>::Create();
     textureManager = FileManager<Texture>::Create();
     worldObjectManager = ObjectManager::Create();
     screenObjectManager = ObjectManager::Create();

     //Load Meshes
     meshManager->LoadFile("model_3D_cube", "resources/models/cube.obj");
     meshManager->LoadFile("model_3D_ship", "resources/models/ship.obj");
     meshManager->LoadFile("model_3D_pyramid", "resources/models/pyramid.obj");
     meshManager->LoadFile("model_2D_square", "resources/models/square.obj");

     //Load Shaders
     shaderManager->LoadFile("shader_lit", "resources/shaders/litCube.vs", "resources/shaders/litCube.fs");
     shaderManager->LoadFile("shader_unlit", "resources/shaders/cube.vs", "resources/shaders/cube.fs");
     shaderManager->LoadFile("shader_square", "resources/shaders/square.vs", "resources/shaders/square.fs");

     //Load Textures
     textureManager->LoadFile("tex_cat", "resources/textures/cat.jpg");
     textureManager->LoadFile("tex_catCube", "resources/textures/catCube.jpg");
     textureManager->LoadFile("tex_catCubeLabeled", "resources/textures/catCubeLabeled.png");

     //Create 3D Objects
     //Mesh, Shader, Texture, Position, Rotation, Scale
     worldObjectManager->LoadObject("unlitCube",
          Object::CreateObject(meshManager->GetValue("model_3D_cube"),
                               shaderManager->GetValue("shader_unlit"),
                               nullptr,
                               glm::vec3(0.0f),
                               glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                               glm::vec3(0.1f)));

     worldObjectManager->LoadObject("catCube",
          Object::CreateObject(meshManager->GetValue("model_3D_cube"),
                               shaderManager->GetValue("shader_lit"),
                               textureManager->GetValue("tex_catCubeLabeled"),
                               glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                               glm::vec3(0.5f)));

     
     worldObjectManager->LoadObject("pyramid",
          Object::CreateObject(meshManager->GetValue("model_3D_pyramid"),
                               shaderManager->GetValue("shader_lit"),
                               textureManager->GetValue("tex_cat"),
                               glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                               glm::vec3(0.25f)));
     
     worldObjectManager->LoadObject("ship",
          Object::CreateObject(meshManager->GetValue("model_3D_ship"),
                               shaderManager->GetValue("shader_lit"),
                               nullptr,
                               glm::vec3(0.0f, 1.0f, 0.0f),
                               glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                               glm::vec3(0.25f)));
     
     //Create 2D Objects
     screenObjectManager->LoadObject("square",
          Object::CreateObject(meshManager->GetValue("model_2D_square"),
                               shaderManager->GetValue("shader_square"),
                               textureManager->GetValue("tex_cat"),
                               glm::vec3(0.4f, 0.4f, 0.0f),
                               glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                               glm::vec3(0.2f)));

     screenObjectManager->LoadObject("pyramid",
          Object::CreateObject(meshManager->GetValue("model_3D_pyramid"),
                               shaderManager->GetValue("shader_square"),
                               textureManager->GetValue("tex_cat"),
                               glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                               glm::vec3(0.25f)));

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
          //unlitCube->Draw3D(camera, glm::vec3(0.0f));
          //
          //litCube->Draw3D(camera, unlitCube->GetPosition());
          //pyramid->Draw3D(camera, unlitCube->GetPosition());
          //ship->Draw3D(camera, unlitCube->GetPosition());

          //auto unlitcube = worldObjectManager->getObject("unlitCube");
          //unlitcube->Draw3D(camera, glm::vec3(0.0f));

          worldObjectManager->Render3DObjects(camera, lightPos);

          //Render 2D Scene
          //pyramid->Draw2D();
          //square->Draw2D();

          screenObjectManager->Render2DObjects();

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
               if (angleIncrement < MAX_SPIN) {
                    if (mods == GLFW_MOD_SHIFT && MAX_SPIN - angleIncrement >= 5.0f) {
                         angleIncrement += 5.0f;
                    }
                    else {
                         angleIncrement += 1.0f;
                    }
                    windowPtr->UpdateTitle("SpinSpeed = " + std::to_string(angleIncrement));
               }
               break;
          //Spin Speed Decrease
          case GLFW_KEY_MINUS:
               if (angleIncrement > MIN_SPIN) {
                    if (mods == GLFW_MOD_SHIFT && angleIncrement - MIN_SPIN >= 5.0f) {
                         angleIncrement -= 5.0f;
                    }
                    else {
                         angleIncrement -= 1.0f;
                    }
                    windowPtr->UpdateTitle("SpinSpeed = " + std::to_string(angleIncrement));
               }
               break;
          //Fullscreen Toggle
          case GLFW_KEY_F:
               windowPtr->ToggleFullscreen();
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

     ////Light Source Control
     if (windowPtr->ProcessInput(GLFW_KEY_J)) { lightPos += (deltaTime * 10.0f) * glm::vec3(1.0f, 0.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_L)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(1.0f, 0.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_K)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(0.0f, 0.0f, 1.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_I)) { lightPos += (deltaTime * 10.0f) * glm::vec3(0.0f, 0.0f, 1.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_U)) { lightPos -= (deltaTime * 10.0f) * glm::vec3(0.0f, 1.0f, 0.0f); }
     if (windowPtr->ProcessInput(GLFW_KEY_O)) { lightPos += (deltaTime * 10.0f) * glm::vec3(0.0f, 1.0f, 0.0f); }
}

void Animate() {
     if (spin) {
          angle += (deltaTime * angleIncrement);
          worldObjectManager->GetObject("catCube")->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, angle));
          worldObjectManager->GetObject("ship")->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, angle));
     }
     worldObjectManager->GetObject("unlitCube")->SetPosition(lightPos);
}


/*
//DEFINES
#define MINSPIN 1.0f
#define MAXSPIN 100.0f

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
*/