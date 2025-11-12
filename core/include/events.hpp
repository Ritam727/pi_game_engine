#pragma once

#include "glm/glm.hpp"

#include <variant>
#include <vector>

namespace core {
  class WindowResizeEvent {
  private:
    int width;
    int height;

  public:
    WindowResizeEvent(int width, int height);

    int getWidth() const;
    int getHeight() const;

    bool operator==(const WindowResizeEvent &event) const;
  };

  class WindowCloseEvent {
  public:
    bool operator==(const WindowCloseEvent &event) const;
  };
}

namespace core {
  enum class InputAction { RELEASE, PRESS, HOLD };
}

namespace core {
  class KeyEvent {
  private:
    int         key;
    InputAction type;

  public:
    KeyEvent(int key, InputAction type);
    ~KeyEvent();

    int         getKey() const;
    InputAction getType() const;

    bool operator==(const KeyEvent &event) const;
  };
}

namespace core {
  class MouseButtonEvent {
  private:
    int         button;
    InputAction type;

  public:
    MouseButtonEvent(int button, InputAction type);

    int         getButton() const;
    InputAction getType() const;

    bool operator==(const MouseButtonEvent &event) const;
  };

  class MouseMovementEvent {
  private:
    double x;
    double y;

  public:
    MouseMovementEvent(double x, double y);

    double getX() const;
    double getY() const;

    bool operator==(const MouseMovementEvent &event) const;
  };

  class MouseScrollEvent {
  private:
    double x;
    double y;

  public:
    MouseScrollEvent(double x, double y);

    double getX() const;
    double getY() const;

    bool operator==(const MouseScrollEvent &event) const;
  };
}

namespace core {
  using Event =
      std::variant<MouseMovementEvent, MouseButtonEvent, MouseScrollEvent,
                   KeyEvent, WindowResizeEvent, WindowCloseEvent>;

  enum class InputEventType {
    MOUSE_MOVEMENT_EVENT,
    MOUSE_BUTTON_EVENT,
    MOUSE_SCROLL_EVENT,
    KEY_EVENT,
    WINDOW_RESIZE_EVENT,
    WINDOW_CLOSE_EVENT
  };

  class InputEvent {
  private:
    InputEventType type;
    Event          data;

  public:
    InputEvent(KeyEvent data);
    InputEvent(MouseButtonEvent data);
    InputEvent(WindowResizeEvent data);
    InputEvent(WindowCloseEvent data);
    InputEvent(MouseMovementEvent data);
    InputEvent(MouseScrollEvent data);

    InputEventType getType();

    const Event &getData() const;

    bool operator==(InputEvent &event);

    static const std::vector<InputEventType> &getEventTypes();
  };
}