#pragma once

#include "inputs_events.hpp"
#include "utils.hpp"
#include "glm/glm.hpp"

namespace inputs {
  iterableEnum(CameraState, DEFAULT, CAMERA_PAN_MODE, CAMERA_FLY_MODE,
               CAMERA_RESET_MODE);
  iterableEnum(FovState, NORMAL, NOT_NORMAL);
  iterableEnum(StateType, CAMERA_STATE, FOV_STATE);

  enum class MousePositionReset { NONE, INIT, DONE };
}

namespace inputs {
  struct InputState {
    std::vector<InputAction> keyboardState{400, InputAction::RELEASE};
    std::vector<InputAction> mousebuttonState{3, InputAction::RELEASE};
    CameraState              cameraState{CameraState::DEFAULT};
    FovState                 fovState{FovState::NORMAL};
    glm::vec2                mousePosition{0.0f, 0.0f};
    float                    scrollDelta{0.0f};

    MousePositionReset resetMousePosition{MousePositionReset::NONE};
  };
}