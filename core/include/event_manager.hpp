#pragma once

#include "events.hpp"
#include "utils.hpp"

#include <array>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace core {
  class InputEventManager {
  private:
    std::vector<InputEventType>                                 inputEventTypes;
    std::vector<std::vector<std::function<void(InputEvent &)>>> subscribers;
    std::vector<std::array<std::vector<InputEvent>, 2>>         topics;

    int read = 0;
    int write = 1;

    InputEventManager();

  public:
    static InputEventManager &getInstance();

    void executeEvents();
    void subscribe(InputEventType                    type,
                   std::function<void(InputEvent &)> handle);
    void enqueue(InputEvent event);
  };
}

namespace core {
  class EventManager {
  private:
    std::unordered_map<
        std::string,
        std::vector<std::function<void(std::unique_ptr<BaseEvent> &)>>>
        subscribers;
    std::unordered_map<std::string, std::vector<std::unique_ptr<BaseEvent>>>
                                                topics;
    std::unordered_set<std::string>             topicNames;
    std::unordered_map<std::string, std::mutex> subscriberMutexes;
    std::unordered_map<std::string, std::mutex> topicMutexes;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents();

    template <IsSubclassOf<BaseEvent> T>
    void enqueue(const std::string &topicName, std::unique_ptr<T> event) {
      this->topicMutexes.try_emplace(topicName);
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