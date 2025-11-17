#pragma once

#include "events.hpp"
#include "utils.hpp"

#include <functional>
#include <mutex>
#include <unordered_map>

namespace core {
  class EventManager {
  private:
    std::unordered_map<
        std::string,
        std::vector<std::function<void(std::unique_ptr<BaseEvent> &)>>>
        subscribers;
    std::unordered_map<std::string, std::vector<std::unique_ptr<BaseEvent>>>
                                                topics;
    std::unordered_map<std::string, std::mutex> subscriberMutexes;
    std::unordered_map<std::string, std::mutex> topicMutexes;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents(const std::vector<std::string> &topics);

    template <core::IsSubclassOf<BaseEvent> T>
    void enqueue(const std::string &topicName, std::unique_ptr<T> event) {
      if (!this->topicMutexes.contains(topicName)) {
        this->topicMutexes.try_emplace(topicName);
      }
      std::lock_guard<std::mutex> lock(this->topicMutexes[topicName]);
      for (std::unique_ptr<BaseEvent> &queuedEvent : this->topics[topicName]) {
        if (static_cast<T>(*event.get()) ==
            *(static_cast<T *>(queuedEvent.get()))) {
          return;
        }
      }
      this->topics[topicName].push_back(std::move(event));
    }

    void subscribe(const std::string                                &topicName,
                   std::function<void(std::unique_ptr<BaseEvent> &)> handle);
  };
}