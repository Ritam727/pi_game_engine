#pragma once

namespace core {
  struct IEvent {
    virtual ~IEvent() = default;
  };
}

namespace core {
  template <typename T> struct Event : public IEvent {
    T data;

    Event(T data) : data(data) {}
  };
};

namespace core {
  enum class InputAction { RELEASE, PRESS, HOLD };
}

namespace core {
  struct WindowResizeEvent {
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

  struct WindowCloseEvent {
    bool operator==(const WindowCloseEvent &event) const {
      return false;
    }
  };

  struct KeyEvent {
    int         key;
    InputAction type;

    KeyEvent(int key, InputAction type) {
      this->key = key;
      this->type = type;
    }

    bool operator==(const KeyEvent &event) const {
      return this->key == event.key && this->type == event.type;
    }
  };

  struct MouseMovementEvent {
    double x;
    double y;

    MouseMovementEvent(double x, double y) {
      this->x = x;
      this->y = y;
    }

    bool operator==(const MouseMovementEvent &event) const {
      return this->x == event.x && this->y == event.y;
    }
  };

  struct MouseButtonEvent {
    int         button;
    InputAction type;

    MouseButtonEvent(int button, InputAction type) {
      this->button = button;
      this->type = type;
    }

    bool operator==(const MouseButtonEvent &event) const {
      return this->button == event.button && this->type == event.type;
    }
  };

  struct MouseScrollEvent {
    double x;
    double y;

    MouseScrollEvent(double x, double y) {
      this->x = x;
      this->y = y;
    }

    bool operator==(const MouseScrollEvent &event) const {
      return this->x == event.x && this->y == event.y;
    }
  };
}