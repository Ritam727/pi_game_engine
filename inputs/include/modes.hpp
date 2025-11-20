#pragma once

#include "utils.hpp"

namespace inputs {
  class IMode {
  public:
    virtual ~IMode() = default;
  };
}

namespace inputs {
  template <core::IsIterableEnum T> class Mode : public IMode {
  protected:
    T mode;

  public:
    Mode() : mode(static_cast<T>(0)) {}

    T getMode() {
      return this->mode;
    }

    void setMode(unsigned int val) {
      this->mode = static_cast<T>(val);
    }

    void incrementMode() {
      this->mode = core::getNextEnumValue<T>(this->mode);
    }
  };
}

namespace inputs {
  ITERABLE_ENUM(CameraViewMode, NORMAL, BIRD_EYE, FISH_EYE);
  ITERABLE_ENUM(CameraMoveMode, STATIC, PAN, MOVE_AROUND, FLY);
}