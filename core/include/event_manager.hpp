#pragma once

#include "logger.hpp"
#include "events.hpp"

#include <array>
#include <functional>
#include <mutex>

namespace core {
  class InputEventManager {
  private:
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