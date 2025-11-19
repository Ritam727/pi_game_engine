#include "window.hpp"
#include "GLFW/glfw3.h"
#include "constants.hpp"
#include "event_manager.hpp"
#include "logger.hpp"

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
    Window::buildGlfwKeyMapping();
  }

  void Window::buildGlfwKeyMapping() {
    Window::glfwKeyMapping[GLFW_KEY_SPACE] = "SPACE";
    Window::glfwKeyMapping[GLFW_KEY_APOSTROPHE] = "APOSTROPHE";
    Window::glfwKeyMapping[GLFW_KEY_COMMA] = "COMMA";
    Window::glfwKeyMapping[GLFW_KEY_MINUS] = "MINUS";
    Window::glfwKeyMapping[GLFW_KEY_PERIOD] = "PERIOD";
    Window::glfwKeyMapping[GLFW_KEY_SLASH] = "SLASH";
    Window::glfwKeyMapping[GLFW_KEY_0] = "0";
    Window::glfwKeyMapping[GLFW_KEY_1] = "1";
    Window::glfwKeyMapping[GLFW_KEY_2] = "2";
    Window::glfwKeyMapping[GLFW_KEY_3] = "3";
    Window::glfwKeyMapping[GLFW_KEY_4] = "4";
    Window::glfwKeyMapping[GLFW_KEY_5] = "5";
    Window::glfwKeyMapping[GLFW_KEY_6] = "6";
    Window::glfwKeyMapping[GLFW_KEY_7] = "7";
    Window::glfwKeyMapping[GLFW_KEY_8] = "8";
    Window::glfwKeyMapping[GLFW_KEY_9] = "9";
    Window::glfwKeyMapping[GLFW_KEY_SEMICOLON] = "SEMICOLON";
    Window::glfwKeyMapping[GLFW_KEY_EQUAL] = "EQUAL";
    Window::glfwKeyMapping[GLFW_KEY_A] = "A";
    Window::glfwKeyMapping[GLFW_KEY_B] = "B";
    Window::glfwKeyMapping[GLFW_KEY_C] = "C";
    Window::glfwKeyMapping[GLFW_KEY_D] = "D";
    Window::glfwKeyMapping[GLFW_KEY_E] = "E";
    Window::glfwKeyMapping[GLFW_KEY_F] = "F";
    Window::glfwKeyMapping[GLFW_KEY_G] = "G";
    Window::glfwKeyMapping[GLFW_KEY_H] = "H";
    Window::glfwKeyMapping[GLFW_KEY_I] = "I";
    Window::glfwKeyMapping[GLFW_KEY_J] = "J";
    Window::glfwKeyMapping[GLFW_KEY_K] = "K";
    Window::glfwKeyMapping[GLFW_KEY_L] = "L";
    Window::glfwKeyMapping[GLFW_KEY_M] = "M";
    Window::glfwKeyMapping[GLFW_KEY_N] = "N";
    Window::glfwKeyMapping[GLFW_KEY_O] = "O";
    Window::glfwKeyMapping[GLFW_KEY_P] = "P";
    Window::glfwKeyMapping[GLFW_KEY_Q] = "Q";
    Window::glfwKeyMapping[GLFW_KEY_R] = "R";
    Window::glfwKeyMapping[GLFW_KEY_S] = "S";
    Window::glfwKeyMapping[GLFW_KEY_T] = "T";
    Window::glfwKeyMapping[GLFW_KEY_U] = "U";
    Window::glfwKeyMapping[GLFW_KEY_V] = "V";
    Window::glfwKeyMapping[GLFW_KEY_W] = "W";
    Window::glfwKeyMapping[GLFW_KEY_X] = "X";
    Window::glfwKeyMapping[GLFW_KEY_Y] = "Y";
    Window::glfwKeyMapping[GLFW_KEY_Z] = "Z";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_BRACKET] = "LEFT_BRACKET";
    Window::glfwKeyMapping[GLFW_KEY_BACKSLASH] = "BACKSLASH";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_BRACKET] = "RIGHT_BRACKET";
    Window::glfwKeyMapping[GLFW_KEY_GRAVE_ACCENT] = "GRAVE_ACCENT";
    Window::glfwKeyMapping[GLFW_KEY_WORLD_1] = "WORLD_1";
    Window::glfwKeyMapping[GLFW_KEY_WORLD_2] = "WORLD_2";
    Window::glfwKeyMapping[GLFW_KEY_ESCAPE] = "ESCAPE";
    Window::glfwKeyMapping[GLFW_KEY_ENTER] = "ENTER";
    Window::glfwKeyMapping[GLFW_KEY_TAB] = "TAB";
    Window::glfwKeyMapping[GLFW_KEY_BACKSPACE] = "BACKSPACE";
    Window::glfwKeyMapping[GLFW_KEY_INSERT] = "INSERT";
    Window::glfwKeyMapping[GLFW_KEY_DELETE] = "DELETE";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT] = "RIGHT";
    Window::glfwKeyMapping[GLFW_KEY_LEFT] = "LEFT";
    Window::glfwKeyMapping[GLFW_KEY_DOWN] = "DOWN";
    Window::glfwKeyMapping[GLFW_KEY_UP] = "UP";
    Window::glfwKeyMapping[GLFW_KEY_PAGE_UP] = "PAGE_UP";
    Window::glfwKeyMapping[GLFW_KEY_PAGE_DOWN] = "PAGE_DOWN";
    Window::glfwKeyMapping[GLFW_KEY_HOME] = "HOME";
    Window::glfwKeyMapping[GLFW_KEY_END] = "END";
    Window::glfwKeyMapping[GLFW_KEY_CAPS_LOCK] = "CAPS_LOCK";
    Window::glfwKeyMapping[GLFW_KEY_SCROLL_LOCK] = "SCROLL_LOCK";
    Window::glfwKeyMapping[GLFW_KEY_NUM_LOCK] = "NUM_LOCK";
    Window::glfwKeyMapping[GLFW_KEY_PRINT_SCREEN] = "PRINT_SCREEN";
    Window::glfwKeyMapping[GLFW_KEY_PAUSE] = "PAUSE";
    Window::glfwKeyMapping[GLFW_KEY_F1] = "F1";
    Window::glfwKeyMapping[GLFW_KEY_F2] = "F2";
    Window::glfwKeyMapping[GLFW_KEY_F3] = "F3";
    Window::glfwKeyMapping[GLFW_KEY_F4] = "F4";
    Window::glfwKeyMapping[GLFW_KEY_F5] = "F5";
    Window::glfwKeyMapping[GLFW_KEY_F6] = "F6";
    Window::glfwKeyMapping[GLFW_KEY_F7] = "F7";
    Window::glfwKeyMapping[GLFW_KEY_F8] = "F8";
    Window::glfwKeyMapping[GLFW_KEY_F9] = "F9";
    Window::glfwKeyMapping[GLFW_KEY_F10] = "F10";
    Window::glfwKeyMapping[GLFW_KEY_F11] = "F11";
    Window::glfwKeyMapping[GLFW_KEY_F12] = "F12";
    Window::glfwKeyMapping[GLFW_KEY_F13] = "F13";
    Window::glfwKeyMapping[GLFW_KEY_F14] = "F14";
    Window::glfwKeyMapping[GLFW_KEY_F15] = "F15";
    Window::glfwKeyMapping[GLFW_KEY_F16] = "F16";
    Window::glfwKeyMapping[GLFW_KEY_F17] = "F17";
    Window::glfwKeyMapping[GLFW_KEY_F18] = "F18";
    Window::glfwKeyMapping[GLFW_KEY_F19] = "F19";
    Window::glfwKeyMapping[GLFW_KEY_F20] = "F20";
    Window::glfwKeyMapping[GLFW_KEY_F21] = "F21";
    Window::glfwKeyMapping[GLFW_KEY_F22] = "F22";
    Window::glfwKeyMapping[GLFW_KEY_F23] = "F23";
    Window::glfwKeyMapping[GLFW_KEY_F24] = "F24";
    Window::glfwKeyMapping[GLFW_KEY_F25] = "F25";
    Window::glfwKeyMapping[GLFW_KEY_KP_0] = "KP_0";
    Window::glfwKeyMapping[GLFW_KEY_KP_1] = "KP_1";
    Window::glfwKeyMapping[GLFW_KEY_KP_2] = "KP_2";
    Window::glfwKeyMapping[GLFW_KEY_KP_3] = "KP_3";
    Window::glfwKeyMapping[GLFW_KEY_KP_4] = "KP_4";
    Window::glfwKeyMapping[GLFW_KEY_KP_5] = "KP_5";
    Window::glfwKeyMapping[GLFW_KEY_KP_6] = "KP_6";
    Window::glfwKeyMapping[GLFW_KEY_KP_7] = "KP_7";
    Window::glfwKeyMapping[GLFW_KEY_KP_8] = "KP_8";
    Window::glfwKeyMapping[GLFW_KEY_KP_9] = "KP_9";
    Window::glfwKeyMapping[GLFW_KEY_KP_DECIMAL] = "KP_DECIMAL";
    Window::glfwKeyMapping[GLFW_KEY_KP_DIVIDE] = "KP_DIVIDE";
    Window::glfwKeyMapping[GLFW_KEY_KP_MULTIPLY] = "KP_MULTIPLY";
    Window::glfwKeyMapping[GLFW_KEY_KP_SUBTRACT] = "KP_SUBTRACT";
    Window::glfwKeyMapping[GLFW_KEY_KP_ADD] = "KP_ADD";
    Window::glfwKeyMapping[GLFW_KEY_KP_ENTER] = "KP_ENTER";
    Window::glfwKeyMapping[GLFW_KEY_KP_EQUAL] = "KP_EQUAL";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_SHIFT] = "SHIFT";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_CONTROL] = "CONTROL";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_ALT] = "ALT";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_SUPER] = "SUPER";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_SHIFT] = "SHIFT";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_CONTROL] = "CONTROL";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_ALT] = "ALT";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_SUPER] = "SUPER";
    Window::glfwKeyMapping[GLFW_MOUSE_BUTTON_LEFT] = "MOUSELEFT";
    Window::glfwKeyMapping[GLFW_MOUSE_BUTTON_RIGHT] = "MOUSE_RIGHT";
    Window::glfwKeyMapping[GLFW_MOUSE_BUTTON_MIDDLE] = "MOUSEMIDDLE";
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
    core::EventManager::getInstance().enqueue<KeyEvent>(
        Constants::KEY_STATE_TOPIC,
        std::make_unique<KeyEvent>(key, static_cast<InputAction>(action)));
    if (key == GLFW_KEY_ESCAPE) {
      core::EventManager::getInstance().enqueue<WindowCloseEvent>(
          Constants::WINDOW_CLOSE_TOPIC, std::make_unique<WindowCloseEvent>());
    }
  }

  void Window::closeCallback(GLFWwindow *window) {
    core::EventManager::getInstance().enqueue<WindowCloseEvent>(
        Constants::WINDOW_CLOSE_TOPIC, std::make_unique<WindowCloseEvent>());
  }

  void Window::mouseMovementCallback(GLFWwindow *window, double x, double y) {
    core::EventManager::getInstance().enqueue<MouseMovementEvent>(
        Constants::MOUSE_MOVEMENT_TOPIC,
        std::make_unique<MouseMovementEvent>(x, y));
  }

  void Window::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                   int mods) {
    core::EventManager::getInstance().enqueue<MouseButtonEvent>(
        Constants::MOUSE_BUTTON_TOPIC,
        std::make_unique<MouseButtonEvent>(button,
                                           static_cast<InputAction>(action)));
  }

  void Window::mouseScrollCallback(GLFWwindow *window, double x, double y) {
    core::EventManager::getInstance().enqueue<MouseScrollEvent>(
        Constants::MOUSE_SCROLL_TOPIC,
        std::make_unique<MouseScrollEvent>(x, y));
  }

  const std::vector<std::string> &Window::getGlfwToKeyMapping() {
    return glfwKeyMapping;
  }
}