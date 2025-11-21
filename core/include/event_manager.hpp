#pragma once

#include "events.hpp"
#include "utils.hpp"

#include <array>
#include <functional>
#include <mutex>
#include <unordered_map>

namespace core {
  using IEventPtr = std::unique_ptr<IEvent>;

  class EventManager {
  private:
    std::unordered_map<std::string,
                       std::vector<std::function<void(IEventPtr &)>>>
        subscribers;
    std::unordered_map<std::string, std::array<std::vector<IEventPtr>, 2>>
                                                  topics;
    std::unordered_map<std::string, unsigned int> readIndexes;
    std::unordered_map<std::string, unsigned int> writeIndexes;
    std::unordered_map<std::string, std::mutex>   subscriberMutexes;
    std::unordered_map<std::string, std::mutex>   swapMutexes;

  public:
    EventManager();

    void executeEvents(const std::vector<std::string> &topics);

    template <typename T, typename... Args>
    void enqueue(const std::string &topicName, Args... args) {
      if (!this->swapMutexes.contains(topicName)) {
        this->swapMutexes.try_emplace(topicName);
      }
      if (!this->readIndexes.contains(topicName)) {
        this->readIndexes[topicName] = 0;
      }
      if (!this->writeIndexes.contains(topicName)) {
        this->writeIndexes[topicName] = 1;
      }
      T event{std::forward<Args>(args)...};
      for (IEventPtr &queuedEvent :
           this->topics[topicName][this->writeIndexes[topicName]]) {
        if (event == (static_cast<Event<T> *>(queuedEvent.get()))->data) {
          return;
        }
      }
      this->topics[topicName][this->writeIndexes[topicName]].push_back(
          std::make_unique<Event<T>>(event));
    }

    void subscribe(const std::string               &topicName,
                   std::function<void(IEventPtr &)> handle);
  };
}