#include "event_manager.hpp"
#include <utility>

namespace core {
  EventManager::EventManager()
      : subscribers(BasicEvent::getEventTypes().size()),
        topics(BasicEvent::getEventTypes().size()) {}

  void EventManager::subscribe(BasicEventType                    type,
                               std::function<void(BasicEvent &)> handle) {
    std::lock_guard<std::mutex> lock = std::lock_guard(subscriberMutex);
    subscribers[static_cast<unsigned long>(type)].emplace_back(
        std::move(handle));
  }

  void EventManager::enqueue(BasicEvent event) {
    std::lock_guard<std::mutex> lock = std::lock_guard(this->writeMutex);
    unsigned long idx = static_cast<unsigned long>(event.getType());
    for (BasicEvent &prevEvent : this->topics[idx][this->write]) {
      if (prevEvent == event) {
        return;
      }
    }
    this->topics[static_cast<unsigned long>(event.getType())][this->write]
        .emplace_back(std::move(event));
  }

  void EventManager::executeEvents() {
    {
      std::lock_guard<std::mutex> lock = std::lock_guard(this->swapMutex);
      this->read = (this->read + 1) % 2;
      this->write = (this->write + 1) % 2;
    }
    for (BasicEventType type : BasicEvent::getEventTypes()) {
      unsigned long            idx = static_cast<unsigned long>(type);
      std::vector<BasicEvent> &events = this->topics[idx][this->read];
      std::vector<std::function<void(BasicEvent &)>> &handles =
          this->subscribers[idx];
      for (int i = 0; i < events.size(); i++) {
        for (std::function<void(BasicEvent &)> &handle : handles) {
          handle(events[i]);
        }
      }
      events.clear();
    }
  }

  EventManager &EventManager::getInstance() {
    static EventManager instance;
    return instance;
  }
}