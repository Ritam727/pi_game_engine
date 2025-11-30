#pragma once

#include "glm/glm.hpp"

namespace ui {
  class CameraControls {
  private:
    static void positionControls(glm::vec3 &position);
    static void angularControls(glm::vec2 &angles);

  public:
    static void cameraControls(glm::vec3 &position, glm::vec2 &angles);
  };
}