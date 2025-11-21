#include "event_manager.hpp"

#include <utility>

namespace core {
  EventManager::EventManager() {}

  void EventManager::executeEvents(const std::vector<std::string> &topics) {
    for (const std::string &topic : topics) {
      std::lock_guard<std::mutex> swapLock(this->swapMutexes[topic]);
      this->readIndexes[topic] ^= 1;
      this->writeIndexes[topic] ^= 1;
    }
    for (const std::string &topic : topics) {
      std::vector<IEventPtr> &events =
          this->topics[topic][this->readIndexes[topic]];
      if (this->subscribers.contains(topic)) {
        std::lock_guard subscriberLock(this->subscriberMutexes[topic]);
        std::vector<std::function<void(IEventPtr &)>> &handles =
            this->subscribers[topic];
        for (std::function<void(IEventPtr &)> &handle : handles) {
          for (unsigned int i = 0; i < events.size(); i++) {
            handle(events[i]);
          }
        }
      }
      events.clear();
    }
  }

  void EventManager::subscribe(const std::string               &topicName,
                               std::function<void(IEventPtr &)> handle) {
    if (!this->subscriberMutexes.contains(topicName)) {
      this->subscriberMutexes.try_emplace(topicName);
    }
    std::lock_guard<std::mutex> lock(this->subscriberMutexes[topicName]);
    this->subscribers[topicName].push_back(std::move(handle));
  }
}