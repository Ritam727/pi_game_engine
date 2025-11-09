#pragma once

#include "events.hpp"

#include <functional>

namespace core {
  class EventHandle {
  private:
    std::function<void(BasicEvent &)> call;

  public:
    EventHandle(std::function<void(BasicEvent &)> call) {
      this->call = call;
    }

    void execute(BasicEvent &e) {
      this->call(std::ref(e));
    }
  };
}