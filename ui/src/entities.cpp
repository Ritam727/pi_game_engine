#include "entities.hpp"

#include "imgui.h"
#include "selectable.hpp"
#include "ui_constants.hpp"

namespace ui {
  Entities::Entities(UIState &uiState) : UILayer(uiState) {}

  void Entities::onUpdate(float ts, core::Registry &registry) {
    Entities::entitiesWindow(this->uiState.selectedEntity, registry);
  }

  void Entities::entitiesWindow(core::Entity   &selectedEntity,
                                core::Registry &registry) {
    const std::vector<core::Entity> &selectableEntities =
        registry.getPool<core::Selectable>().getEntities();

    ImGui::Begin(Constants::ENTITIES.c_str());

    if (ImGui::Button(Constants::ADD_ENTITY.c_str())) {
      core::Entity newEntity = registry.createEntity();
      registry.addComponent<core::Selectable>(newEntity);
    }
    for (unsigned int i = 0;
         i < registry.getPool<core::Selectable>().getNumElements(); i++) {
      const core::Entity &entity = selectableEntities[i];
      std::string         str = Constants::ENTITY + std::to_string(entity);

      if (ImGui::Selectable(str.c_str(), selectedEntity == entity)) {
        selectedEntity = entity;
      }
      if (ImGui::BeginPopupContextItem()) {
        ImGui::PushID(str.c_str());
        if (ImGui::Button(Constants::DELETE.c_str())) {
          registry.removeEntity(entity);
          ImGui::CloseCurrentPopup();
        }
        ImGui::PopID();
        ImGui::EndPopup();
      }
    }
    ImGui::End();
  }
}