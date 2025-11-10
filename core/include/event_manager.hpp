#pragma once

#include "logger.hpp"
#include "events.hpp"

#include <array>
#include <functional>
#include <mutex>

namespace core {
  class EventManager {
  private:
    std::vector<std::vector<std::function<void(BasicEvent &)>>> subscribers;
    std::vector<std::array<std::vector<BasicEvent>, 2>>         topics;

    std::mutex subscriberMutex;
    std::mutex queueMutex;

    int read = 0;
    int write = 1;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents();
    void subscribe(BasicEventType                    type,
                   std::function<void(BasicEvent &)> handle);
    void enqueue(BasicEvent event);
  };
}