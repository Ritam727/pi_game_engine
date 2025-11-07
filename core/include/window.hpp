#pragma once

#include "event_manager.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "event_manager.hpp"
#include "events.hpp"
#include "logger.hpp"
#include <memory>
#include <stdexcept>

namespace core {
  class Window {
  private:
    GLFWwindow *window;

    static void framebufferResizeCallback(GLFWwindow *window, int width,
                                          int height);
    static void keyCallback(GLFWwindow *window, int key, int scanCode,
                            int action, int mods);
    static void closeCallback(GLFWwindow *window);

  public:
    Window(int width, int height, char const *windowName);
    ~Window();

    void processGlfwFrame();
  };
}