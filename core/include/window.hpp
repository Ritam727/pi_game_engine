#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>
#include <vector>

namespace core {
  class Window {
  private:
    GLFWwindow *window;

    static void framebufferResizeCallback(GLFWwindow *window, int width,
                                          int height);
    static void keyCallback(GLFWwindow *window, int key, int scanCode,
                            int action, int mods);
    static void closeCallback(GLFWwindow *window);
    static void mouseMovementCallback(GLFWwindow *window, double x, double y);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                    int mods);
    static void mouseScrollCallback(GLFWwindow *window, double x, double y);
    static void buildGlfwKeyMapping();

    static inline std::vector<std::string> glfwKeyMapping{350, std::string{}};

  public:
    Window(int width, int height, const std::string &name);
    ~Window();

    void processGlfwFrame();
    void pollEvents();
    void setCursorVisibility(bool visibility);

    static const std::vector<std::string> &getGlfwToKeyMapping();
  };
}