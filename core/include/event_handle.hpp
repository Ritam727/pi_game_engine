#pragma once

#include "event.hpp"

#include <functional>
#include <memory>

namespace events {
  class EventHandle {
  private:
    std::function<void(std::unique_ptr<BaseEvent> &)> call;

  public:
    EventHandle(std::function<void(std::unique_ptr<BaseEvent> &)> call) {
      this->call = call;
    }

    void execute(std::unique_ptr<BaseEvent> &e) {
      this->call(std::ref(e));
    }
  };
}