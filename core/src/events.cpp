#include "events.hpp"

namespace core {
  WindowResizeEvent::WindowResizeEvent(int width, int height) {
    this->width = width;
    this->height = height;
  }

  int WindowResizeEvent::getWidth() const {
    return this->width;
  }

  int WindowResizeEvent::getHeight() const {
    return this->height;
  }

  bool WindowResizeEvent::operator==(const WindowResizeEvent &event) const {
    return (this->getWidth() == event.getWidth() &&
            this->getHeight() == event.getHeight());
  }

  bool WindowCloseEvent::operator==(const WindowCloseEvent &event) const {
    return false;
  }
}

namespace core {
  KeyEvent::KeyEvent(int key, InputAction type) : key(key), type(type) {}

  KeyEvent::~KeyEvent() {}

  int KeyEvent::getKey() const {
    return this->key;
  }

  InputAction KeyEvent::getType() const {
    return this->type;
  }

  bool KeyEvent::operator==(const KeyEvent &event) const {
    return this->key == event.getKey() && this->type == event.getType();
  }
}

namespace core {
  MouseButtonEvent::MouseButtonEvent(int button, InputAction type)
      : button(button), type(type) {}

  int MouseButtonEvent::getButton() const {
    return this->button;
  }

  InputAction MouseButtonEvent::getType() const {
    return this->type;
  }

  bool MouseButtonEvent::operator==(const MouseButtonEvent &event) const {
    return this->button == event.getButton() && this->type == event.getType();
  }

  MouseMovementEvent::MouseMovementEvent(double x, double y) : x(x), y(y) {}

  double MouseMovementEvent::getX() const {
    return this->x;
  }

  double MouseMovementEvent::getY() const {
    return this->y;
  }

  bool MouseMovementEvent::operator==(const MouseMovementEvent &event) const {
    return this->x == event.getX() && this->y == event.getY();
  }

  MouseScrollEvent::MouseScrollEvent(double x, double y) : x(x), y(y) {}

  double MouseScrollEvent::getX() const {
    return this->x;
  }

  double MouseScrollEvent::getY() const {
    return this->y;
  }

  bool MouseScrollEvent::operator==(const MouseScrollEvent &event) const {
    return this->x == event.getX() && this->y == event.getY();
  }
}

namespace core {
  InputEvent::InputEvent(KeyEvent data)
      : type(InputEventType::KEY_EVENT), data(data) {}

  InputEvent::InputEvent(MouseButtonEvent data)
      : type(InputEventType::MOUSE_BUTTON_EVENT), data(data) {}

  InputEvent::InputEvent(WindowResizeEvent data)
      : type(InputEventType::WINDOW_RESIZE_EVENT), data(data) {}

  InputEvent::InputEvent(WindowCloseEvent data)
      : type(InputEventType::WINDOW_CLOSE_EVENT), data(data) {}

  InputEvent::InputEvent(MouseMovementEvent data)
      : type(InputEventType::MOUSE_MOVEMENT_EVENT), data(data) {}

  InputEvent::InputEvent(MouseScrollEvent data)
      : type(InputEventType::MOUSE_SCROLL_EVENT), data(data) {}

  InputEventType InputEvent::getType() {
    return this->type;
  }

  const Event &InputEvent::getData() const {
    return this->data;
  }

  bool InputEvent::operator==(InputEvent &event) {
    return this->type == event.getType() && this->data == event.getData();
  }

  const std::vector<InputEventType> &InputEvent::getEventTypes() {
    static std::vector<InputEventType> eventTypes{
        InputEventType::MOUSE_MOVEMENT_EVENT,
        InputEventType::MOUSE_BUTTON_EVENT,
        InputEventType::MOUSE_SCROLL_EVENT,
        InputEventType::KEY_EVENT,
        InputEventType::WINDOW_RESIZE_EVENT,
        InputEventType::WINDOW_CLOSE_EVENT};
    return eventTypes;
  }
}