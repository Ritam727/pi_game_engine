#include "event_manager.hpp"

#include <utility>

namespace core {
  EventManager::EventManager() {}

  EventManager &EventManager::getInstance() {
    static EventManager eventManager;
    return eventManager;
  }

  void EventManager::executeEvents(const std::vector<std::string> &topics) {
    core::logger::info("{}", topics.size());
    for (const std::string &topic : topics) {
      core::logger::info("Handling topic {}", topic);
      std::lock_guard topicLock(this->topicMutexes[topic]);
      std::vector<std::unique_ptr<BaseEvent>> &events = this->topics[topic];
      if (this->subscribers.contains(topic)) {
        std::lock_guard subscriberLock(this->subscriberMutexes[topic]);
        std::vector<std::function<void(std::unique_ptr<BaseEvent> &)>>
            &handles = this->subscribers[topic];
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