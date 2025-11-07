#pragma once

#include <memory>

namespace events {
  class BaseEvent {
  public:
    virtual bool equals(std::unique_ptr<BaseEvent> &event) {
      return true;
    }

    bool operator==(std::unique_ptr<BaseEvent> &event) {
      return this->equals(std::ref(event));
    }

    virtual ~BaseEvent() = default;
  };
}