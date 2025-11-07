#pragma once

#include "event_handle.hpp"

#include <typeindex>
#include <unordered_map>

namespace events {
  class EventManager {
  private:
    std::unordered_map<std::type_index,
                       std::vector<std::unique_ptr<EventHandle>>>
        subscribers;
    std::unordered_map<std::type_index, std::vector<std::unique_ptr<BaseEvent>>>
        topics;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents();

    template <typename T> void subscribe(std::unique_ptr<EventHandle> handle) {
      std::type_index idx = std::type_index(typeid(T));
      subscribers[idx].push_back(std::move(handle));
    }

    template <typename T> void enqueue(std::unique_ptr<BaseEvent> e) {
      std::type_index idx = std::type_index(typeid(T));
      // if (topics.contains(idx)) {
      //   for (std::unique_ptr<BaseEvent> &event : topics[idx]) {
      //     if (*(event.get()) == e) {
      //       return;
      //     }
      //   }
      // }
      topics[idx].push_back(std::move(e));
    }

    template <typename T> std::vector<std::unique_ptr<BaseEvent>> &getQueue() {
      return this->topics[std::type_index(typeid(T))];
    }
  };
}