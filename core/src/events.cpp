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
  BasicEvent::BasicEvent(KeyEvent data)
      : type(BasicEventType::KEY_EVENT), data(data) {}

  BasicEvent::BasicEvent(MouseEvent data)
      : type(BasicEventType::MOUSE_EVENT), data(data) {}

  BasicEvent::BasicEvent(WindowResizeEvent data)
      : type(BasicEventType::WINDOW_RESIZE_EVENT), data(data) {}

  BasicEvent::BasicEvent(WindowCloseEvent data)
      : type(BasicEventType::WINDOW_CLOSE_EVENT), data(data) {}

  BasicEventType BasicEvent::getType() {
    return this->type;
  }

  const std::variant<BASIC_EVENTS> &BasicEvent::getData() const {
    return this->data;
  }

  bool BasicEvent::operator==(BasicEvent &event) {
    return this->type == event.getType() && this->data == event.getData();
  }

  const std::vector<BasicEventType> &BasicEvent::getEventTypes() {
    static std::vector<BasicEventType> eventTypes{
        BasicEventType::KEY_EVENT, BasicEventType::MOUSE_EVENT,
        BasicEventType::WINDOW_RESIZE_EVENT,
        BasicEventType::WINDOW_CLOSE_EVENT};
    return eventTypes;
  }
}