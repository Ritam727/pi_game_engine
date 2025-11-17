#pragma once

#include "events.hpp"

namespace inputs {
  enum class InputAction { RELEASE, PRESS, HOLD };
}

namespace inputs {
  struct WindowResizeEvent : public core::BaseEvent {
    int width;
    int height;

    bool operator==(const WindowResizeEvent &event) const {
      return this->width == event.width && this->height == event.height;
    }
  };

  struct WindowCloseEvent : public core::BaseEvent {
    bool operator==(const WindowCloseEvent &event) const {
      return false;
    }
  };

  struct KeyEvent : public core::BaseEvent {
    int         key;
    InputAction type;

    bool operator==(const KeyEvent &event) const {
      return this->key == event.key && this->type == event.type;
    }
  };

  struct MouseMovementEvent : public core::BaseEvent {
    double x;
    double y;

    bool operator==(const MouseMovementEvent &event) const {
      return this->x == event.x && this->y == event.y;
    }
  };

  struct MouseButtonEvent : public core::BaseEvent {
    int         button;
    InputAction type;

    bool operator==(const MouseButtonEvent &event) const {
      return this->button == event.button && this->type == event.type;
    }
  };

  struct MouseScrollEvent : public core::BaseEvent {
    double x;
    double y;

    bool operator==(const MouseScrollEvent &event) const {
      return this->x == event.x && this->y == event.y;
    }
  };
}

namespace inputs {
  struct FovChangeEvent : public core::BaseEvent {
    float fov;

    FovChangeEvent(float fov) {
      this->fov = fov;
    }

    bool operator==(const FovChangeEvent &event) const {
      return this->fov == fov;
    }
  };
}