#pragma once

namespace gl {
  struct FovChangeEvent {
    float fov;

    FovChangeEvent(float fov) {
      this->fov = fov;
    }

    bool operator==(const FovChangeEvent &event) const {
      return this->fov == event.fov;
    }
  };
}