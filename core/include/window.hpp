#pragma once

#include "event_manager.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>
#include <vector>

namespace core {
  class Window {
  private:
    EventManager &eventManager;
    GLFWwindow   *window;

    void registerKeyCallback();
    void registerMouseButtonCallback();
    void registerWindowResizeCallback();
    void registerWindowCloseCallback();
    void registerMouseMoveCallback();
    void registerMouseScrollCallback();
    void registerMouseVisibilityCallback();

    static void buildGlfwKeyMapping();

    static inline std::vector<std::string> glfwKeyMapping{350, std::string{}};

  public:
    Window(int width, int height, const std::string &name,
           EventManager &eventManager);
    ~Window();

    void processGlfwFrame();
    void pollEvents();
    void setCursorVisibility(bool visibility);

    static const std::vector<std::string> &getGlfwToKeyMapping();
  };
}