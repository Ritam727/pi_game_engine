#include "light_controls.hpp"
#include "commons.hpp"
#include "imgui.h"
#include "ui_constants.hpp"

namespace ui {
  void LightControls::directionalLightControls(const core::Entity &entity,
                                               gl::LightComponent &light) {
    ImGui::DragFloat3(Constants::DIRECTION.c_str(), &light.direction.x,
                      Constants::SPEED_MEDIUM);
  }

  void LightControls::pointLightControls(const core::Entity &entity,
                                         gl::LightComponent &light) {
    ImGui::DragFloat3(Constants::POSITION.c_str(), &light.position.x,
                      Constants::SPEED_FAST);
    ImGui::DragFloat(Constants::LINEAR.c_str(), &light.linear,
                     Constants::SPEED_SLOW);
    ImGui::DragFloat(Constants::QUADRATIC.c_str(), &light.quadratic,
                     Constants::SPEED_SLOW);
  }

  void LightControls::spotLightControls(const core::Entity &entity,
                                        gl::LightComponent &light) {
    ImGui::DragFloat3(Constants::DIRECTION.c_str(), &light.direction.x,
                      Constants::SPEED_MEDIUM);
    ImGui::DragFloat3(Constants::POSITION.c_str(), &light.position.x,
                      Constants::SPEED_FAST);
    ImGui::DragFloat(Constants::LINEAR.c_str(), &light.linear,
                     Constants::SPEED_SLOW);
    ImGui::DragFloat(Constants::QUADRATIC.c_str(), &light.quadratic,
                     Constants::SPEED_SLOW);
    ImGui::DragFloat(Constants::INNER_CUT_OFF.c_str(), &light.innerCutOff,
                     Constants::SPEED_MEDIUM, Constants::ZERO,
                     light.outerCutOff - Constants::SPEED_FAST);
    ImGui::DragFloat(Constants::OUTER_CUT_OFF.c_str(), &light.outerCutOff,
                     Constants::SPEED_MEDIUM,
                     light.innerCutOff + Constants::SPEED_FAST,
                     Constants::ANGLE_MAX);
  }

  void LightControls::lightComponent(const core::Entity &entity,
                                     gl::LightComponent &light) {
    std::string ID{Constants::LIGHT_COMPONENT + std::to_string(entity)};
    const std::vector<std::string> &items = Constants::LIGHT_TYPES;

    ImGui::PushID(ID.c_str());
    Commons::dropDownMenu<gl::LightType>(Constants::TYPE_LABEL, items,
                                         light.type);
    Commons::colorControls(light.ambient, light.diffuse, light.specular);

    switch (light.type) {
    case gl::LightType::DIRECTIONAL_LIGHT:
      LightControls::directionalLightControls(entity, light);
      break;
    case gl::LightType::POINT_LIGHT:
      LightControls::pointLightControls(entity, light);
      break;
    case gl::LightType::SPOT_LIGHT:
      LightControls::spotLightControls(entity, light);
      break;
    default:
      break;
    }

    ImGui::PopID();
  }
}