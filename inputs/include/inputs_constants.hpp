#pragma once

#include <string>

namespace inputs {
  class Constants {
  public:
    static const inline unsigned int KEYBOARD_SHORTCUT_DEPTH{5};

    static const inline char UNDERSCORE{'_'};

    static const inline std::string FOV_CHANGE_TOPIC{"events.renderer.fov"};
    static const inline std::string EMPTY_STRING{""};
  };
}