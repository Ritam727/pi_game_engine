#pragma once

#include <string>

namespace gl {
  class Constants {
  public:
    static constexpr float SPEED_SCALAR{0.06f};

    static const inline std::string WINDOW_RESIZE_TOPIC{"events.window.resize"};
    static const inline std::string FOV_CHANGE_TOPIC{"events.renderer.fov"};
    static const inline std::string MODEL_LOAD_TOPIC{"events.models.load"};
  };
}