#pragma once

#include "mode_manager.hpp"
#include "glm/glm.hpp"
#include <functional>

namespace inputs {
  struct State {
    CameraViewMode cameraViewMode{CameraViewMode::NORMAL};
    glm::vec2      mousePosition{0.0f, 0.0f};

    bool resetMousePosition{false};
  };

  struct InputState {
    std::vector<unsigned int> buttonsPressed;
  };

  struct Activation {
    const std::function<void(ModeManager &, std::function<void()>)> onPress;
    const std::function<void(ModeManager &, std::function<void()>)> onRelease;
    bool activated{false};
  };
}