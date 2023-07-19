#include "window.h"

#include <stb_image.h>

Window::Window(const std::string& title, int width, int height, void(*frameBufferCallback), void(*keyCallback), bool fullscreen) {
     glfwSetErrorCallback([](int code, const char* msg) {throw msg;});

     windowWidth = width;
     windowHeight = height;
     monitorWidth = 0;
     monitorHeight = 0;

     glfwInit();
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
     glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

     this->window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);
     if (this->window == NULL) {
          fprintf(stderr, "Failed to create GLFW window\n");
          glfwTerminate();
          exit(-1);
     }

     glfwMakeContextCurrent(window);
     glfwSetFramebufferSizeCallback(this->window, static_cast<GLFWframebuffersizefun>(frameBufferCallback));
     glfwSetKeyCallback(this->window, static_cast<GLFWkeyfun>(keyCallback));

     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
          fprintf(stderr, "Failed to initialize GLAD\n");
          exit(-1);
     }

     glfwSwapInterval(0);

     monitor = glfwGetPrimaryMonitor();
     fullscreenMode = fullscreen;

     glfwGetMonitorPhysicalSize(monitor, &monitorWidth, &monitorHeight);

     SetFullscreen();
}

Window::~Window() {
     glfwDestroyWindow(this->window);
     glfwTerminate();
}

std::shared_ptr<Window> Window::CreateWindow(const std::string& title, int width, int height, void(*frameBufferCallback), void(*keyCallback), bool fullscreen) {
     std::shared_ptr<Window> window = std::shared_ptr<Window>(new Window(title, width, height, frameBufferCallback, keyCallback, fullscreen));

     GLFWimage image;
     image.pixels = stbi_load("resources/textures/cat.jpg", &image.width, &image.height, NULL, 4);
     glfwSetWindowIcon(window->window, 1, &image);
     stbi_image_free(image.pixels);

     return window;
}

void Window::UpdateTitle(const std::string& title) {
     windowTitle = title;
     glfwSetWindowTitle(this->window, windowTitle.c_str());
}

void Window::AppendTitle(const std::string& appendage) {
     windowTitle += appendage;
     glfwSetWindowTitle(this->window, windowTitle.c_str());
}

void Window::Flush() {
     glfwSwapBuffers(this->window);
     glfwPollEvents();
}

bool Window::ProcessInput(int key) {
     if (glfwGetKey(this->window, key) == GLFW_PRESS) return true;
     return false;
}

void Window::Close() {
     glfwSetWindowShouldClose(this->window, true);
}

bool Window::ShouldClose() {
     return glfwWindowShouldClose(this->window);
}

void Window::ToggleFullscreen() {
     fullscreenMode = !fullscreenMode;
     SetFullscreen();
}

void Window::SetFullscreen() {
     const GLFWvidmode* mode = glfwGetVideoMode(this->monitor);

     if (fullscreenMode) {
          glfwSetWindowMonitor(this->window, this->monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
     } else {
          glfwSetWindowMonitor(this->window, NULL, monitorWidth / 2, monitorHeight/2, windowWidth, windowHeight, mode->refreshRate);
     }
}