#include "event_manager.hpp"

#include <utility>

namespace core {
  InputEventManager::InputEventManager()
      : subscribers(InputEvent::getEventTypes().size()),
        topics(InputEvent::getEventTypes().size()) {}

  void InputEventManager::subscribe(InputEventType                    type,
                                    std::function<void(InputEvent &)> handle) {
    subscribers[static_cast<unsigned long>(type)].emplace_back(
        std::move(handle));
  }

  void InputEventManager::enqueue(InputEvent event) {
    unsigned long idx = static_cast<unsigned long>(event.getType());
    for (InputEvent &prevEvent : this->topics[idx][this->write]) {
      if (prevEvent == event) {
        return;
      }
    }
    this->topics[static_cast<unsigned long>(event.getType())][this->write]
        .emplace_back(std::move(event));
  }

  void InputEventManager::executeEvents() {
    this->read = (this->read + 1) % 2;
    this->write = (this->write + 1) % 2;
    for (InputEventType type : InputEvent::getEventTypes()) {
      unsigned long            idx = static_cast<unsigned long>(type);
      std::vector<InputEvent> &events = this->topics[idx][this->read];
      std::vector<std::function<void(InputEvent &)>> &handles =
          this->subscribers[idx];
      for (int i = 0; i < events.size(); i++)
        for (std::function<void(InputEvent &)> &handle : handles)
          handle(events[i]);
      events.clear();
    }
  }

  InputEventManager &InputEventManager::getInstance() {
    static InputEventManager instance;
    return instance;
  }
}