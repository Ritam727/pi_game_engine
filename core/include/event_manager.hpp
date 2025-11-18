#pragma once

#include "events.hpp"
#include "utils.hpp"

#include <array>
#include <functional>
#include <mutex>
#include <unordered_map>

namespace core {
  using BaseEventPtr = std::unique_ptr<BaseEvent>;

  class EventManager {
  private:
    std::unordered_map<std::string,
                       std::vector<std::function<void(BaseEventPtr &)>>>
        subscribers;
    std::unordered_map<std::string, std::array<std::vector<BaseEventPtr>, 2>>
                                                  topics;
    std::unordered_map<std::string, unsigned int> readIndexes;
    std::unordered_map<std::string, unsigned int> writeIndexes;
    std::unordered_map<std::string, std::mutex>   subscriberMutexes;
    std::unordered_map<std::string, std::mutex>   swapMutexes;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents(const std::vector<std::string> &topics);

    template <core::IsSubClassOf<BaseEvent> T>
    void enqueue(const std::string &topicName, std::unique_ptr<T> event) {
      if (!this->swapMutexes.contains(topicName)) {
        this->swapMutexes.try_emplace(topicName);
      }
      if (!this->readIndexes.contains(topicName)) {
        this->readIndexes[topicName] = 0;
      }
      if (!this->writeIndexes.contains(topicName)) {
        this->writeIndexes[topicName] = 1;
      }
      std::lock_guard<std::mutex> lock(this->swapMutexes[topicName]);
      for (BaseEventPtr &queuedEvent :
           this->topics[topicName][this->writeIndexes[topicName]]) {
        if (static_cast<T>(*event.get()) ==
            *(static_cast<T *>(queuedEvent.get()))) {
          return;
        }
      }
      this->topics[topicName][this->writeIndexes[topicName]].push_back(
          std::move(event));
    }

    void subscribe(const std::string                  &topicName,
                   std::function<void(BaseEventPtr &)> handle);
  };
}