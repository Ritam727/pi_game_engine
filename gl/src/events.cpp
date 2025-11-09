#include "events.hpp"

namespace core {
  bool WindowEvent::equals(std::unique_ptr<events::BaseEvent> &event) {
    return false;
  }

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

  bool WindowResizeEvent::equals(std::unique_ptr<events::BaseEvent> &event) {
    WindowResizeEvent *windowEvent = (WindowResizeEvent *) event.get();
    return (this->getWidth() == windowEvent->getWidth() &&
            this->getHeight() == windowEvent->getHeight());
  }

  KeyEvent::KeyEvent(Type type) {
    this->type = type;
  }

  KeyEvent::~KeyEvent() {}

  KeyEvent::Type KeyEvent::getType() {
    return this->type;
  }

  bool KeyEvent::equals(std::unique_ptr<events::BaseEvent> &event) {
    return this->type == static_cast<KeyEvent *>(event.get())->getType();
  }
}