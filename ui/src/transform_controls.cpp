#include "transform_controls.hpp"
#include "imgui.h"
#include "ui_constants.hpp"

namespace ui {
  void TransformControls::transformControls(glm::vec3 &translation,
                                            glm::vec3 &rotation,
                                            glm::vec3 &scale) {
    TransformControls::translationControls(translation);
    TransformControls::rotationControls(rotation);
    TransformControls::scaleControls(scale);
  }

  void TransformControls::translationControls(glm::vec3 &translation) {
    ImGui::DragFloat3(Constants::TRANSLATION.c_str(), &translation.x,
                      Constants::SPEED_MEDIUM);
  }

  void TransformControls::rotationControls(glm::vec3 &rotation) {
    ImGui::DragFloat3(Constants::ROTATION.c_str(), &rotation.x,
                      Constants::SPEED_FAST);
  }

  void TransformControls::scaleControls(glm::vec3 &scale) {
    ImGui::DragFloat3(Constants::SCALE.c_str(), &scale.x,
                      Constants::SPEED_MEDIUM);
  }
}