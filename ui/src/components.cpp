#include "components.hpp"

#include "camera_controls.hpp"
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
  Components::Components(UIState &uiState) : UILayer(uiState) {}

  void Components::onUpdate(float ts, core::Registry &registry) {
    this->componentsWindow(registry);
  }

  void Components::componentsWindow(core::Registry &registry) {
    ImGui::Begin(Constants::COMPONENTS.c_str());

    Components::componentControls<core::Transform>(
        registry, Constants::TRANSFORM_COMPONENT, this->uiState.selectedEntity,
        [&]() {
          core::Transform &transform = registry.getPool<core::Transform>().get(
              this->uiState.selectedEntity);
          glm::vec3 previousRotation = this->uiState.rotation;

          TransformControls::transformControls(
              transform.position, this->uiState.rotation, transform.scale);

          transform.updateRotation(this->uiState.rotation - previousRotation);
        });

    Components::componentControls<core::CameraTransform>(
        registry, Constants::CAMERA_TRANSFORM_COMPONENT,
        this->uiState.selectedEntity, [&]() {
          core::CameraTransform &cameraTransform =
              registry.getPool<core::CameraTransform>().get(
                  this->uiState.selectedEntity);
          CameraControls::cameraControls(cameraTransform.position,
                                         cameraTransform.angles,
                                         cameraTransform.cameraActive);
        });

    Components::componentControls<core::LightComponent>(
        registry, Constants::LIGHT_CONTROLS_LABEL, this->uiState.selectedEntity,
        [&]() {
          LightControls::lightComponent(
              this->uiState.selectedEntity,
              registry.getPool<core::LightComponent>().get(
                  this->uiState.selectedEntity));
        });

    ImGui::End();
  }
}