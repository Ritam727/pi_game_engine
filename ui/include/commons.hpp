#pragma once

#include "glm/glm.hpp"
#include "utils.hpp"
#include "imgui.h"

#include <string>

namespace ui {
  class Commons {
  public:
    template <core::IsIterableEnum E>
    static void dropDownMenu(const std::string                &label,
                             const std::array<std::string, 3> &items, E &val) {
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
      ImGui::DragFloat3("Ambient", &ambient.x, 0.01, 0.0f, 1.0f);
      ImGui::DragFloat3("Diffuse", &diffuse.x, 0.01, 0.0f, 1.0f);
      ImGui::DragFloat3("Specular", &specular.x, 0.01, 0.0f, 1.0f);
    }
  };
}