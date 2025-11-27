#include "components.hpp"

#include "imgui.h"
#include "registry.hpp"
#include "transform.hpp"
#include "camera_transform.hpp"
#include "light_controls.hpp"

namespace ui {
  void Components::componentsWindow(
      core::SparseSet<core::Entity> &selectedEntities,
      core::Registry                &registry) {
    std::vector<core::Entity> &entities = selectedEntities.getEntities();

    ImGui::Begin("Components");
    for (unsigned int i = 0; i < selectedEntities.getNumElements(); i++) {
      core::Entity &entity = entities[i];
      std::string   str;
      if (registry.getPool<core::Transform>().contains(entity)) {
        str = "Transform " + std::to_string(entity);
        ImGui::Text("%s", str.c_str());
      }
      if (registry.getPool<core::CameraTransform>().contains(entity)) {
        str = "Camera Transform " + std::to_string(entity);
        ImGui::Text("%s", str.c_str());
      }
      if (registry.getPool<gl::LightComponent>().contains(entity)) {
        LightControls::lightComponent(
            entity, registry.getPool<gl::LightComponent>().get(entity));
      }
    }
    ImGui::End();
  }
}