#include "camera_controls.hpp"
#include "imgui.h"
#include "ui_constants.hpp"

namespace ui {
  void CameraControls::cameraControls(glm::vec3 &position, glm::vec2 &angles) {
    CameraControls::positionControls(position);
    CameraControls::angularControls(angles);
  }

  void CameraControls::positionControls(glm::vec3 &position) {
    ImGui::DragFloat3("Position", &position.x, Constants::SPEED_MEDIUM);
  }

  void CameraControls::angularControls(glm::vec2 &angles) {
    ImGui::DragFloat2("Angles", &angles.x, Constants::SPEED_MEDIUM);
  }
}