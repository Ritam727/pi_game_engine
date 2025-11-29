#include "components.hpp"

#include "imgui.h"
#include "lights.hpp"
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

    ImGui::Begin(Constants::COMPONENTS.c_str());
    for (unsigned int i = 0; i < selectedEntities.getNumElements(); i++) {
      const core::Entity &entity = entities[i];
      std::string         str;
      Components::componentControls<core::Transform>(
          registry, Constants::TRANSFORM_COMPONENT, entity, [&]() {
            core::Transform &transform =
                registry.getPool<core::Transform>().get(entity);
            glm::vec3 previousTranslation = this->uiState.translation;
            glm::vec3 previousRotation = this->uiState.rotation;
            glm::vec3 previousScale = this->uiState.scale;

            TransformControls::transformControls(this->uiState.translation,
                                                 this->uiState.rotation,
                                                 this->uiState.scale);

            transform.updatePosition(this->uiState.translation -
                                     previousTranslation);
            transform.updateRotation(this->uiState.rotation - previousRotation);
            transform.updateScale(this->uiState.scale - previousScale);
          });
      Components::componentControls<core::CameraTransform>(
          registry, Constants::CAMERA_TRANSFORM_COMPONENT, entity, [&]() {});
      Components::componentControls<gl::LightComponent>(
          registry, Constants::LIGHT_CONTROLS_LABEL, entity, [&]() {
            LightControls::lightComponent(
                entity, registry.getPool<gl::LightComponent>().get(entity));
          });
    }
    ImGui::End();
  }
}