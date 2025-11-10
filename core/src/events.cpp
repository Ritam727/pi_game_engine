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
  KeyEvent::KeyEvent(Type type) {
    this->type = type;
  }

  KeyEvent::~KeyEvent() {}

  KeyEvent::Type KeyEvent::getType() const {
    return this->type;
  }

  bool KeyEvent::operator==(const KeyEvent &event) const {
    return this->type == event.getType();
  }
}

namespace core {
  bool MouseEvent::operator==(const MouseEvent &event) const {
    return false;
  }
}

namespace core {
  InputEvent::InputEvent(KeyEvent data)
      : type(InputEventType::KEY_EVENT), data(data) {}

  InputEvent::InputEvent(MouseEvent data)
      : type(InputEventType::MOUSE_EVENT), data(data) {}

  InputEvent::InputEvent(WindowResizeEvent data)
      : type(InputEventType::WINDOW_RESIZE_EVENT), data(data) {}

  InputEvent::InputEvent(WindowCloseEvent data)
      : type(InputEventType::WINDOW_CLOSE_EVENT), data(data) {}

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
        InputEventType::MOUSE_EVENT, InputEventType::KEY_EVENT,
        InputEventType::WINDOW_RESIZE_EVENT,
        InputEventType::WINDOW_CLOSE_EVENT};
    return eventTypes;
  }
}