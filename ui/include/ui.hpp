#pragma once

#include "layer.hpp"
#include "lights.hpp"
#include "registry.hpp"
#include "transform.hpp"
#include "utils.hpp"
#include "window.hpp"
#include "imgui.h"

namespace ui {
  class UI : public core::Layer {
  private:
    core::Window                 &window;
    core::Registry               &registry;
    core::SparseSet<core::Entity> selectedEntities{};

    void dragFloat3(const std::string &ID, const std::string &label,
                    glm::vec3 &value, float delta = 0.01);
    void dragFloat(const std::string &ID, const std::string &label,
                   float &value, float delta = 0.01);

    template <core::IsIterableEnum E>
    void dropDownMenu(std::vector<std::string> &items, const std::string &label,
                      E &val) {
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

    void colorControls(core::Entity &entity, glm::vec3 &ambient,
                       glm::vec3 &diffuse, glm::vec3 &specular);

    void directionalLightControls(core::Entity       &entity,
                                  gl::LightComponent &lightComponent);
    void pointLightControls(core::Entity       &entity,
                            gl::LightComponent &lightComponent);
    void spotLightControls(core::Entity       &entity,
                           gl::LightComponent &lightComponent);

  public:
    UI(core::Window &window, core::Registry &registry);
    ~UI();

    void onUpdate(float ts) override;
    void postUpdate() override;

    void createDockspace();
    void entitiesWindow();
    void componentsWindow();

    void lightComponent(core::Entity       &entity,
                        gl::LightComponent &lightComponent);
  };
}