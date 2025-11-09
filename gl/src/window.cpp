#include "window.hpp"
#include "GLFW/glfw3.h"
#include "event_manager.hpp"
#include "events.hpp"

#include <stdexcept>

namespace gl {
  Window::Window(int width, int height, const std::string &name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    if (window == nullptr) {
      core::logger::error("Failed to create window");
      glfwTerminate();
      throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      core::logger::error("Failed to initialize GLAD");
      throw std::runtime_error("Failed to initialize GLAD");
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, Window::framebufferResizeCallback);
    glfwSetKeyCallback(window, Window::keyCallback);
    glfwSetWindowCloseCallback(window, Window::closeCallback);
  }

  Window::~Window() {
    glfwTerminate();
  }

  void Window::processGlfwFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  void Window::framebufferResizeCallback(GLFWwindow *window, int width,
                                         int height) {
    core::EventManager::getInstance().enqueue(
        core::BasicEvent(core::WindowResizeEvent(width, height)));
    glViewport(0, 0, width, height);
  }

  void Window::keyCallback(GLFWwindow *window, int key, int scanCode,
                           int action, int mods) {
    core::EventManager::getInstance().enqueue(core::BasicEvent(
        core::KeyEvent(static_cast<core::KeyEvent::Type>(key))));
    if (key == GLFW_KEY_ESCAPE) {
      core::EventManager::getInstance().enqueue(
          core::BasicEvent(core::WindowCloseEvent()));
    }
  }

  void Window::closeCallback(GLFWwindow *window) {
    core::EventManager::getInstance().enqueue(
        core::BasicEvent(core::WindowCloseEvent()));
  }
}