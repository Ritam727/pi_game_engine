#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "logger.hpp"

namespace gl {
  class Window {
  private:
    GLFWwindow *window;

    static void framebufferResizeCallback(GLFWwindow *window, int width,
                                          int height);
    static void keyCallback(GLFWwindow *window, int key, int scanCode,
                            int action, int mods);
    static void closeCallback(GLFWwindow *window);

  public:
    Window(int width, int height, const std::string &name);
    ~Window();

    void processGlfwFrame();
  };
}