#pragma once

#include "glm/glm.hpp"
#include "utils.hpp"

#include <variant>
#include <vector>

namespace core {
  struct WindowResizeEvent {
    int width;
    int height;

    bool operator==(const WindowResizeEvent &event) const {
      return (this->width == event.width && this->height == event.height);
    }
  };

  struct WindowCloseEvent {
    bool operator==(const WindowCloseEvent &event) const {
      return false;
    }
  };
}

namespace core {
  enum class InputAction { RELEASE, PRESS, HOLD };
}

namespace core {
  struct KeyEvent {
    int         key;
    InputAction type;

    bool operator==(const KeyEvent &event) const {
      return this->key == event.key && this->type == event.type;
    }
  };
}

namespace core {
  struct MouseButtonEvent {
    int         button;
    InputAction type;

    bool operator==(const MouseButtonEvent &event) const {
      return this->button == event.button && this->type == event.type;
    }
  };

  struct MouseMovementEvent {
    double x;
    double y;

    bool operator==(const MouseMovementEvent &event) const {
      return this->x == event.x && this->y == event.y;
    }
  };

  struct MouseScrollEvent {
    double x;
    double y;

    bool operator==(const MouseScrollEvent &event) const {
      return this->x == event.x && this->y == event.y;
    }
  };
}

namespace core {
  using Event =
      std::variant<MouseMovementEvent, MouseButtonEvent, MouseScrollEvent,
                   KeyEvent, WindowResizeEvent, WindowCloseEvent>;

  iterableEnum(InputEventType, MOUSE_MOVEMENT_EVENT, MOUSE_BUTTON_EVENT,
               MOUSE_SCROLL_EVENT, KEY_EVENT, WINDOW_RESIZE_EVENT,
               WINDOW_CLOSE_EVENT);
}

namespace core {
  struct InputEvent {
    InputEventType type;
    Event          data;

    bool operator==(InputEvent &event) {
      return this->type == event.type && this->data == event.data;
    }
  };
}

namespace core {
  struct BaseEvent {
    virtual ~BaseEvent() = default;
  };

  struct FovChangeEvent : public BaseEvent {
    float fov;
  };
}