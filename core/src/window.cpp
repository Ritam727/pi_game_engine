#include "window.hpp"
#include "GLFW/glfw3.h"
#include "constants.hpp"
#include "event_manager.hpp"
#include "logger.hpp"

#include <stdexcept>

namespace core {
  Window::Window(int width, int height, const std::string &name,
                 EventManager &eventManager)
      : eventManager(eventManager) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);

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

    this->registerKeyCallback();
    this->registerWindowResizeCallback();
    this->registerWindowCloseCallback();
    this->registerMouseButtonCallback();
    this->registerMouseMoveCallback();
    this->registerMouseScrollCallback();
    this->registerMouseVisibilityCallback();

    Window::buildGlfwKeyMapping();
  }

  void Window::registerMouseVisibilityCallback() {
    this->eventManager.subscribe(
        Constants::MOUSE_VISIBLE_TOPIC, [&](IEventPtr &event) {
          MouseVisibleEvent mouseVisibleEvent =
              (static_cast<Event<MouseVisibleEvent> *>(event.get()))->data;
          glfwSetInputMode(this->window, GLFW_CURSOR,
                           mouseVisibleEvent.visible ? GLFW_CURSOR_NORMAL
                                                     : GLFW_CURSOR_DISABLED);
        });
  }

  void Window::registerKeyCallback() {
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scanCode,
                                  int action, int mods) {
      Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      if (self == nullptr)
        return;
      self->eventManager.enqueue<KeyEvent>(
          Constants::KEY_STATE_TOPIC,
          KeyEvent{key, static_cast<InputAction>(action)});
      if (key == GLFW_KEY_ESCAPE) {
        self->eventManager.enqueue<WindowCloseEvent>(
            Constants::WINDOW_CLOSE_TOPIC);
      }
    });
  }

  void Window::registerMouseButtonCallback() {
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button,
                                          int action, int mods) {
      Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      if (self == nullptr)
        return;
      self->eventManager.enqueue<MouseButtonEvent>(
          Constants::MOUSE_BUTTON_TOPIC, button,
          static_cast<InputAction>(action));
    });
  }

  void Window::registerWindowResizeCallback() {
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width,
                                              int height) {
      Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      if (self == nullptr)
        return;
      self->eventManager.enqueue<WindowResizeEvent>(
          Constants::WINDOW_RESIZE_TOPIC, width, height);
    });
  }

  void Window::registerWindowCloseCallback() {
    glfwSetWindowCloseCallback(window, [](GLFWwindow *window) {
      Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      if (self == nullptr)
        return;
      self->eventManager.enqueue<WindowCloseEvent>(
          Constants::WINDOW_CLOSE_TOPIC);
    });
  }

  void Window::registerMouseMoveCallback() {
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x,
                                        double y) {
      Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      if (self == nullptr)
        return;
      self->eventManager.enqueue<MouseMovementEvent>(
          Constants::MOUSE_MOVEMENT_TOPIC, x, y);
    });
  }

  void Window::registerMouseScrollCallback() {
    glfwSetScrollCallback(window, [](GLFWwindow *window, double x, double y) {
      Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      if (self == nullptr)
        return;
      self->eventManager.enqueue<MouseScrollEvent>(
          Constants::MOUSE_SCROLL_TOPIC, x, y);
    });
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
    Window::glfwKeyMapping[GLFW_KEY_LEFT_BRACKET] = "LEFTBRACKET";
    Window::glfwKeyMapping[GLFW_KEY_BACKSLASH] = "BACKSLASH";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_BRACKET] = "RIGHTBRACKET";
    Window::glfwKeyMapping[GLFW_KEY_GRAVE_ACCENT] = "GRAVEACCENT";
    Window::glfwKeyMapping[GLFW_KEY_WORLD_1] = "WORLD1";
    Window::glfwKeyMapping[GLFW_KEY_WORLD_2] = "WORLD2";
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
    Window::glfwKeyMapping[GLFW_KEY_PAGE_UP] = "PAGEUP";
    Window::glfwKeyMapping[GLFW_KEY_PAGE_DOWN] = "PAGEDOWN";
    Window::glfwKeyMapping[GLFW_KEY_HOME] = "HOME";
    Window::glfwKeyMapping[GLFW_KEY_END] = "END";
    Window::glfwKeyMapping[GLFW_KEY_CAPS_LOCK] = "CAPSLOCK";
    Window::glfwKeyMapping[GLFW_KEY_SCROLL_LOCK] = "SCROLLLOCK";
    Window::glfwKeyMapping[GLFW_KEY_NUM_LOCK] = "NUMLOCK";
    Window::glfwKeyMapping[GLFW_KEY_PRINT_SCREEN] = "PRINTSCREEN";
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
    Window::glfwKeyMapping[GLFW_KEY_KP_0] = "KP0";
    Window::glfwKeyMapping[GLFW_KEY_KP_1] = "KP1";
    Window::glfwKeyMapping[GLFW_KEY_KP_2] = "KP2";
    Window::glfwKeyMapping[GLFW_KEY_KP_3] = "KP3";
    Window::glfwKeyMapping[GLFW_KEY_KP_4] = "KP4";
    Window::glfwKeyMapping[GLFW_KEY_KP_5] = "KP5";
    Window::glfwKeyMapping[GLFW_KEY_KP_6] = "KP6";
    Window::glfwKeyMapping[GLFW_KEY_KP_7] = "KP7";
    Window::glfwKeyMapping[GLFW_KEY_KP_8] = "KP8";
    Window::glfwKeyMapping[GLFW_KEY_KP_9] = "KP9";
    Window::glfwKeyMapping[GLFW_KEY_KP_DECIMAL] = "KPDECIMAL";
    Window::glfwKeyMapping[GLFW_KEY_KP_DIVIDE] = "KPDIVIDE";
    Window::glfwKeyMapping[GLFW_KEY_KP_MULTIPLY] = "KPMULTIPLY";
    Window::glfwKeyMapping[GLFW_KEY_KP_SUBTRACT] = "KPSUBTRACT";
    Window::glfwKeyMapping[GLFW_KEY_KP_ADD] = "KPADD";
    Window::glfwKeyMapping[GLFW_KEY_KP_ENTER] = "KPENTER";
    Window::glfwKeyMapping[GLFW_KEY_KP_EQUAL] = "KPEQUAL";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_SHIFT] = "SHIFT";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_CONTROL] = "CONTROL";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_ALT] = "ALT";
    Window::glfwKeyMapping[GLFW_KEY_LEFT_SUPER] = "SUPER";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_SHIFT] = "SHIFT";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_CONTROL] = "CONTROL";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_ALT] = "ALT";
    Window::glfwKeyMapping[GLFW_KEY_RIGHT_SUPER] = "SUPER";
    Window::glfwKeyMapping[GLFW_MOUSE_BUTTON_LEFT] = "MOUSELEFT";
    Window::glfwKeyMapping[GLFW_MOUSE_BUTTON_RIGHT] = "MOUSERIGHT";
    Window::glfwKeyMapping[GLFW_MOUSE_BUTTON_MIDDLE] = "MOUSEMIDDLE";
    Window::glfwKeyMapping[GLFW_MOUSE_BUTTON_4] = "MOUSESCROLL";
  }

  const std::vector<std::string> &Window::getGlfwToKeyMapping() {
    return glfwKeyMapping;
  }
}