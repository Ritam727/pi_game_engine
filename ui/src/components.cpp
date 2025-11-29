#include "components.hpp"

#include "imgui.h"
#include "registry.hpp"
#include "sparse_set.hpp"
#include "transform.hpp"
#include "camera_transform.hpp"
#include "light_controls.hpp"
#include "transform_controls.hpp"
#include "ui_constants.hpp"

namespace ui {
  Components::Components(UIState &uiState) : uiState(uiState) {}

  void Components::onUpdate(float                          ts,
                            core::SparseSet<core::Entity> &selectedEntities,
                            core::Registry                &registry) {
    this->componentsWindow(selectedEntities, registry);
  }

  void Components::componentsWindow(
      core::SparseSet<core::Entity> &selectedEntities,
      core::Registry                &registry) {
    const std::vector<core::Entity> &entities = selectedEntities.getEntities();
    core::ExtendedSparseSet<core::Entity, core::Transform> &transformSet =
        registry.getPool<core::Transform>();

    glm::vec3 previousTranslation = this->uiState.translation;
    glm::vec3 previousRotation = this->uiState.rotation;
    glm::vec3 previousScale = this->uiState.scale;

    ImGui::Begin(Constants::COMPONENTS.c_str());
    if (transformSet.intersects(selectedEntities)) {
      ImGui::Text("%s", Constants::TRANSFORM_COMPONENT.c_str());
      TransformControls::transformControls(this->uiState.translation,
                                           this->uiState.rotation,
                                           this->uiState.scale);
    } else {
      previousTranslation = this->uiState.translation =
          glm::vec3{Constants::ZERO};
      previousRotation = this->uiState.rotation = glm::vec3{Constants::ZERO};
      previousScale = this->uiState.scale = glm::vec3{Constants::ONE};
    }

    for (unsigned int i = 0; i < selectedEntities.getNumElements(); i++) {
      const core::Entity &entity = entities[i];
      std::string         str;
      if (registry.getPool<core::Transform>().contains(entity)) {
        str = Constants::TRANSFORM_COMPONENT + std::to_string(entity);
        core::Transform &transform =
            registry.getPool<core::Transform>().get(entity);
        transform.updatePosition(this->uiState.translation -
                                 previousTranslation);
        transform.updateRotation(this->uiState.rotation - previousRotation);
        transform.updateScale(this->uiState.scale - previousScale);
      }
      if (registry.getPool<core::CameraTransform>().contains(entity)) {
        str = Constants::CAMERA_TRANSFORM_COMPONENT + std::to_string(entity);
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