#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>
#include <GLFW\glfw3.h>

class Window {
private:
     GLFWwindow* window;
     std::string windowTitle;
     int windowWidth, windowHeight;

     GLFWmonitor* monitor;
     int monitorWidth, monitorHeight;
     bool fullscreenMode;

     Window(const std::string&, int width, int height, void(*frameBufferCallback), void(*keyCallback), bool fullscreen);

public:
     ~Window();

     static std::shared_ptr<Window> CreateWindow(const std::string& title, int width, int height, void(*frameBufferCallback), void(*keyCallback), bool fullscreen);

     void UpdateTitle(const std::string& title);
     void AppendTitle(const std::string& appendage);

     void Flush();

     bool ProcessInput(int key);

     void Close();
     bool ShouldClose();

     void ToggleFullscreen();
     void SetFullscreen();
};