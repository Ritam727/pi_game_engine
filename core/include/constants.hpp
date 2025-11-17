#pragma once

#include <string>

namespace core {
  struct Constants {
    static constexpr float SPEED_SCALAR{0.06f};

    static const inline std::string WINDOW_RESIZE_TOPIC{"events.window.resize"};
    static const inline std::string WINDOW_CLOSE_TOPIC{"events.window.close"};
    static const inline std::string KEY_STATE_TOPIC{"events.keys.state"};
    static const inline std::string MOUSE_MOVEMENT_TOPIC{
        "events.mouse.movement"};
    static const inline std::string MOUSE_BUTTON_TOPIC{"events.mouse.button"};
    static const inline std::string MOUSE_SCROLL_TOPIC{"events.mouse.scroll"};
  };
}