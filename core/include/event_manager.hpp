#pragma once

#include "logger.hpp"
#include "events.hpp"
#include "utils.hpp"

#include <array>
#include <functional>
#include <mutex>
#include <typeindex>
#include <unordered_map>

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
        std::type_index,
        std::vector<std::function<void(std::unique_ptr<BaseEvent> &)>>>
        subscribers;
    std::unordered_map<std::type_index, std::vector<std::unique_ptr<BaseEvent>>>
                                                    topics;
    std::unordered_map<std::type_index, std::mutex> subscriberMutexes;
    std::unordered_map<std::type_index, std::mutex> topicMutexes;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents();

    template <IsSubclassOf<BaseEvent> T>
    void enqueue(std::unique_ptr<T> &event) {
      std::type_index idx = std::type_index(typeid(T));
      if (!this->topicMutexes.contains(idx)) {
        this->topicMutexes.emplace(idx, std::mutex{});
      }
      std::lock_guard<std::mutex> lock(this->topicMutexes[idx]);
      this->topics[idx].emplace_back(event);
    }

    template <IsSubclassOf<BaseEvent> T>
    void subscribe(std::function<void(std::unique_ptr<T> &)> &handle) {
      std::type_index idx = std::type_index(typeid(T));
      if (!this->subscriberMutexes.contains(idx)) {
        this->subscriberMutexes.emplace(idx, std::mutex{});
      }
      std::lock_guard<std::mutex> lock(this->subscriberMutexes[idx]);
      this->subscribers[idx].emplace_back(handle);
    }
  };
}