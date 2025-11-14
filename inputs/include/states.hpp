#pragma once

#include "utils.hpp"

namespace inputs {
  iterableEnum(CameraState, DEFAULT, CAMERA_PAN_MODE, CAMERA_FLY_MODE,
               CAMERA_RESET_MODE);
  iterableEnum(StateType, CAMERA_STATE);
}

namespace inputs {
  struct State {
    CameraState cameraState;
  };
}