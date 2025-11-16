#pragma once

#include <string>

namespace core {
  struct Constants {
    static constexpr float SPEED_SCALAR{0.06f};

    static const inline std::string WINDOW_RESIZE_TOPIC{"events.window.resize"};
  };
}