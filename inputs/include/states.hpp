#pragma once

#include "inputs_events.hpp"
#include "utils.hpp"
#include "glm/glm.hpp"
#include <functional>

namespace inputs {
  ITERABLE_ENUM(CameraState, DEFAULT, CAMERA_PAN_MODE, CAMERA_FLY_MODE,
                CAMERA_RESET_MODE);
  ITERABLE_ENUM(FovState, NORMAL, NOT_NORMAL);
  ITERABLE_ENUM(StateType, CAMERA_STATE, FOV_STATE);

  enum class MousePositionReset { NONE, INIT, DONE };
}

namespace inputs {
  struct State {
    std::vector<InputAction> keyboardState{400, InputAction::RELEASE};
    std::vector<InputAction> mousebuttonState{3, InputAction::RELEASE};
    CameraState              cameraState{CameraState::DEFAULT};
    FovState                 fovState{FovState::NORMAL};
    glm::vec2                mousePosition{0.0f, 0.0f};
    float                    scrollDelta{0.0f};

    MousePositionReset resetMousePosition{MousePositionReset::NONE};
  };

  struct InputState {
    unsigned int              currentIndex{0};
    std::vector<unsigned int> buttonsPressed;
  };

  struct Activation {
    const std::function<void()> onPress;
    const std::function<void()> onRelease;
    bool                        activated{false};
  };
}