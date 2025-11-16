#include "event_manager.hpp"
#include "utils.hpp"

#include <utility>

namespace core {
  InputEventManager::InputEventManager()
      : inputEventTypes(enumValues(InputEventType)),
        subscribers(inputEventTypes.size()), topics(inputEventTypes.size()) {}

  void InputEventManager::subscribe(InputEventType                    type,
                                    std::function<void(InputEvent &)> handle) {
    subscribers[static_cast<unsigned long>(type)].emplace_back(
        std::move(handle));
  }

  void InputEventManager::enqueue(InputEvent event) {
    unsigned long idx = static_cast<unsigned long>(event.type);
    for (InputEvent &prevEvent : this->topics[idx][this->write]) {
      if (prevEvent == event) {
        return;
      }
    }
    this->topics[static_cast<unsigned long>(event.type)][this->write]
        .emplace_back(std::move(event));
  }

  void InputEventManager::executeEvents() {
    this->read = (this->read + 1) % 2;
    this->write = (this->write + 1) % 2;
    for (InputEventType &type : this->inputEventTypes) {
      unsigned long            idx = static_cast<unsigned long>(type);
      std::vector<InputEvent> &events = this->topics[idx][this->read];
      std::vector<std::function<void(InputEvent &)>> &handles =
          this->subscribers[idx];
      for (int i = 0; i < events.size(); i++) {
        for (std::function<void(InputEvent &)> &handle : handles) {
          handle(events[i]);
        }
      }
      events.clear();
    }
  }

  InputEventManager &InputEventManager::getInstance() {
    static InputEventManager inputEventManager;
    return inputEventManager;
  }
}

namespace core {
  EventManager::EventManager() {}

  EventManager &EventManager::getInstance() {
    static EventManager eventManager;
    return eventManager;
  }

  void EventManager::executeEvents() {
    for (std::pair<const std::string, std::vector<std::unique_ptr<BaseEvent>>>
             &p : this->topics) {
      const std::string &idx = p.first;
      std::lock_guard    topicLock(this->topicMutexes[idx]);
      std::vector<std::unique_ptr<BaseEvent>> &events = p.second;
      if (this->subscribers.contains(idx)) {
        std::lock_guard subscriberLock(this->subscriberMutexes[idx]);
        std::vector<std::function<void(std::unique_ptr<BaseEvent> &)>>
            &handles = this->subscribers.at(idx);
        for (std::function<void(std::unique_ptr<BaseEvent> &)> &handle :
             handles) {
          for (unsigned int i = 0; i < events.size(); i++) {
            handle(events[i]);
          }
        }
      }
      events.clear();
    }
  }

  void EventManager::subscribe(
      const std::string                                &topicName,
      std::function<void(std::unique_ptr<BaseEvent> &)> handle) {
    if (!this->subscriberMutexes.contains(topicName)) {
      this->subscriberMutexes.try_emplace(topicName);
    }
    std::lock_guard<std::mutex> lock(this->subscriberMutexes[topicName]);
    this->subscribers[topicName].push_back(std::move(handle));
  }
}