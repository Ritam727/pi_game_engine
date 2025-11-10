#pragma once

#include "logger.hpp"
#include "events.hpp"

#include <array>
#include <functional>
#include <mutex>

namespace core {
  class EventManager {
  private:
    std::vector<std::vector<std::function<void(InputEvent &)>>> subscribers;
    std::vector<std::array<std::vector<InputEvent>, 2>>         topics;

    std::mutex subscriberMutex;
    std::mutex queueMutex;

    int read = 0;
    int write = 1;

    EventManager();

  public:
    static EventManager &getInstance();

    void executeEvents();
    void subscribe(InputEventType                    type,
                   std::function<void(InputEvent &)> handle);
    void enqueue(InputEvent event);
  };
}