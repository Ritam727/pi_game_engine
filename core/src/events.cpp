#include "events.hpp"

namespace core {
  bool WindowEvent::equals(std::unique_ptr<events::BaseEvent> &event) {
    return false;
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