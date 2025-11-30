#include "camera_controls.hpp"
#include "imgui.h"
#include "ui_constants.hpp"

namespace ui {
  void CameraControls::cameraControls(glm::vec3 &position, glm::vec2 &angles,
                                      bool &isActive) {
    CameraControls::positionControls(position);
    CameraControls::angularControls(angles);
    CameraControls::activeControls(isActive);
  }

  void CameraControls::positionControls(glm::vec3 &position) {
    ImGui::DragFloat3(Constants::POSITION.c_str(), &position.x,
                      Constants::SPEED_MEDIUM);
  }

  void CameraControls::angularControls(glm::vec2 &angles) {
    ImGui::DragFloat2(Constants::ROTATION.c_str(), &angles.x,
                      Constants::SPEED_MEDIUM);
  }

  void CameraControls::activeControls(bool &isActive) {
    ImGui::Checkbox(Constants::ACTIVE.c_str(), &isActive);
  }
}