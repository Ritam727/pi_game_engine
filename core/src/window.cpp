#include "window.hpp"
#include "GLFW/glfw3.h"
#include "core_constants.hpp"
#include "event_manager.hpp"
#include "events.hpp"

#include <stdexcept>

namespace core {
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
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      core::logger::error("Failed to initialize GLAD");
      throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSetFramebufferSizeCallback(window, Window::framebufferResizeCallback);
    glfwSetKeyCallback(window, Window::keyCallback);
    glfwSetWindowCloseCallback(window, Window::closeCallback);
    glfwSetCursorPosCallback(window, Window::mouseMovementCallback);
    glfwSetMouseButtonCallback(window, Window::mouseButtonCallback);
    glfwSetScrollCallback(window, Window::mouseScrollCallback);
  }

  Window::~Window() {
    glfwTerminate();
  }

  void Window::processGlfwFrame() {
    glfwSwapBuffers(window);
  }

  void Window::pollEvents() {
    glfwWaitEventsTimeout(0.001);
  }

  void Window::setCursorVisibility(bool visibility) {
    glfwSetInputMode(window, GLFW_CURSOR,
                     visibility ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  }

  void Window::framebufferResizeCallback(GLFWwindow *window, int width,
                                         int height) {
    std::unique_ptr<WindowResizeEvent> event =
        std::make_unique<WindowResizeEvent>(width, height);
    core::EventManager::getInstance().enqueue<WindowResizeEvent>(
        Constants::WINDOW_RESIZE_TOPIC, std::move(event));
  }

  void Window::keyCallback(GLFWwindow *window, int key, int scanCode,
                           int action, int mods) {
    core::InputEventManager::getInstance().enqueue(core::InputEvent(
        core::InputEventType::KEY_EVENT,
        core::KeyEvent(key, static_cast<core::InputAction>(action))));
    if (key == GLFW_KEY_ESCAPE) {
      core::InputEventManager::getInstance().enqueue(core::InputEvent(
          core::InputEventType::WINDOW_CLOSE_EVENT, core::WindowCloseEvent()));
    }
  }

  void Window::closeCallback(GLFWwindow *window) {
    core::InputEventManager::getInstance().enqueue(core::InputEvent(
        core::InputEventType::WINDOW_CLOSE_EVENT, core::WindowCloseEvent()));
  }

  void Window::mouseMovementCallback(GLFWwindow *window, double x, double y) {
    core::InputEventManager::getInstance().enqueue(
        core::InputEvent(core::InputEventType::MOUSE_MOVEMENT_EVENT,
                         core::MouseMovementEvent(x, y)));
  }

  void Window::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                   int mods) {
    core::InputEventManager::getInstance().enqueue(
        core::InputEvent(core::InputEventType::MOUSE_BUTTON_EVENT,
                         core::MouseButtonEvent(
                             button, static_cast<core::InputAction>(action))));
  }

  void Window::mouseScrollCallback(GLFWwindow *window, double x, double y) {
    core::InputEventManager::getInstance().enqueue(
        core::InputEvent(core::InputEventType::MOUSE_SCROLL_EVENT,
                         core::MouseScrollEvent(x, y)));
  }
}