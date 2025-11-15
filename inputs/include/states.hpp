#pragma once

#include "events.hpp"
#include "utils.hpp"
#include "glm/glm.hpp"

namespace inputs {
  iterableEnum(CameraState, DEFAULT, CAMERA_PAN_MODE, CAMERA_FLY_MODE,
               CAMERA_RESET_MODE);
  iterableEnum(StateType, CAMERA_STATE);

  enum class MousePositionReset { NONE, INIT, DONE };
}

namespace inputs {
  struct InputState {
    std::vector<core::InputAction> keyboardState{400,
                                                 core::InputAction::RELEASE};
    std::vector<core::InputAction> mousebuttonState{3,
                                                    core::InputAction::RELEASE};
    CameraState                    cameraState{CameraState::DEFAULT};
    glm::vec2                      mousePosition{0.0f, 0.0f};
    float                          scrollDelta{0.0f};

    MousePositionReset resetMousePosition{MousePositionReset::NONE};
  };
}