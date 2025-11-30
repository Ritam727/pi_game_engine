#pragma once

#include "glm/glm.hpp"
#include "registry.hpp"
#include "ui_constants.hpp"
#include "utils.hpp"
#include "imgui.h"

#include <string>

namespace ui {
  struct UIState {
    glm::vec3 translation{Constants::ZERO};
    glm::vec3 rotation{Constants::ZERO};
    glm::vec3 scale{Constants::ONE};

    glm::vec3 cameraPosition{Constants::ZERO};
    glm::vec2 cameraAngles{Constants::ZERO};

    core::Entity selectedEntity{-1};
  };
}

namespace ui {
  class Commons {
  public:
    template <core::IsIterableEnum E>
    static void dropDownMenu(const std::string              &label,
                             const std::vector<std::string> &items, E &val) {
      unsigned int currentIndex = static_cast<unsigned int>(val);
      if (ImGui::BeginCombo(label.c_str(), items[currentIndex].c_str())) {
        for (int i = 0; i < items.size(); i++) {
          bool isSelected = (static_cast<unsigned int>(val) == i);
          if (ImGui::Selectable(items[i].c_str(), isSelected))
            val = static_cast<E>(i);
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
    }

    static void colorControls(glm::vec3 &ambient, glm::vec3 &diffuse,
                              glm::vec3 &specular) {
      ImGui::DragFloat3(Constants::AMBIENT.c_str(), &ambient.x,
                        Constants::SPEED_MEDIUM, Constants::ZERO,
                        Constants::ONE);
      ImGui::DragFloat3(Constants::DIFFUSE.c_str(), &diffuse.x,
                        Constants::SPEED_MEDIUM, Constants::ZERO,
                        Constants::ONE);
      ImGui::DragFloat3(Constants::SPECULAR.c_str(), &specular.x,
                        Constants::SPEED_MEDIUM, Constants::ZERO,
                        Constants::ONE);
    }
  };
}