#pragma once

#include "event_handle.hpp"
#include "logger.hpp"

#include <array>
#include <typeindex>
#include <unordered_map>
#include <mutex>

namespace events {
  class EventManager {
  private:
    std::unordered_map<std::type_index,
                       std::vector<std::unique_ptr<EventHandle>>>
        subscribers;
    std::unordered_map<std::type_index,
                       std::array<std::vector<std::unique_ptr<BaseEvent>>, 2>>
        topics;

    std::mutex swapMutex;
    std::mutex subscriberMutex;
    std::mutex writeMutex;

    int read = 0;
    int write = 1;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents();

    template <typename T> void subscribe(std::unique_ptr<EventHandle> handle) {
      std::lock_guard<std::mutex> lock = std::lock_guard(subscriberMutex);
      std::type_index             idx = std::type_index(typeid(T));
      subscribers[idx].push_back(std::move(handle));
    }

    template <typename T> void enqueue(std::unique_ptr<BaseEvent> e) {
      std::lock_guard<std::mutex> lock = std::lock_guard(this->writeMutex);
      std::type_index             idx = std::type_index(typeid(T));
      if (topics.contains(idx)) {
        for (std::unique_ptr<BaseEvent> &event :
             this->topics[idx][this->write]) {
          if (*(event.get()) == e) {
            return;
          }
        }
      }
      this->topics[idx][this->write].push_back(std::move(e));
    }

    template <typename T>
    std::vector<std::unique_ptr<BaseEvent>> &getReadQueue() {
      return this->topics[std::type_index(typeid(T))][this->read];
    }

    template <typename T>
    std::vector<std::unique_ptr<BaseEvent>> &getWriteQueue() {
      return this->topics[std::type_index(typeid(T))][this->write];
    }
  };
}