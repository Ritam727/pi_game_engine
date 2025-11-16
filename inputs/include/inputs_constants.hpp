#pragma once

#include <string>

namespace inputs {
  class Constants {
  public:
    static constexpr float SPEED_SCALAR{0.06f};

    static const inline std::string FOV_CHANGE_TOPIC{"events.renderer.fov"};
  };
}