#pragma once

#include "event_handle.hpp"
#include "logger.hpp"

#include <array>
#include <memory>
#include <mutex>

namespace core {
  class EventManager {
  private:
    std::vector<std::vector<std::unique_ptr<EventHandle>>> subscribers;
    std::vector<std::array<std::vector<BasicEvent>, 2>>    topics;

    std::mutex swapMutex;
    std::mutex subscriberMutex;
    std::mutex writeMutex;

    int read = 0;
    int write = 1;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents();
    void subscribe(BasicEventType type, std::unique_ptr<EventHandle> handle);
    void enqueue(BasicEvent event);
  };
}