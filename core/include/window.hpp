#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "events.hpp"
#include "logger.hpp"

namespace core {
  struct WindowResizeEvent : public BaseEvent {
    int width;
    int height;

    WindowResizeEvent(int width, int height) {
      this->width = width;
      this->height = height;
    }

    bool operator==(const WindowResizeEvent &event) const {
      return this->width == event.width && this->height == event.height;
    }
  };
}

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

  public:
    Window(int width, int height, const std::string &name);
    ~Window();

    void processGlfwFrame();
    void pollEvents();
    void setCursorVisibility(bool visibility);
  };
}