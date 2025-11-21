#pragma once

namespace inputs {
  struct FovChangeEvent {
    float fov;

    FovChangeEvent(float fov) : fov(fov) {}

    bool operator==(const FovChangeEvent &event) const {
      return this->fov == fov;
    }
  };
}