#pragma once

#include "events.hpp"

namespace inputs {
  struct FovChangeEvent : public core::BaseEvent {
    float fov;

    FovChangeEvent(float fov) {
      this->fov = fov;
    }

    bool operator==(const FovChangeEvent &event) const {
      return this->fov == fov;
    }
  };
}