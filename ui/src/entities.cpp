#include "entities.hpp"

#include "imgui.h"
#include "selectable.hpp"
#include "ui_constants.hpp"

namespace ui {
  void Entities::onUpdate(float                          ts,
                          core::SparseSet<core::Entity> &selectedEntities,
                          core::Registry                &registry) {
    Entities::entitiesWindow(selectedEntities, registry);
  }

  void Entities::entitiesWindow(core::SparseSet<core::Entity> &selectedEntities,
                                core::Registry                &registry) {
    const std::vector<core::Entity> &selectableEntities =
        registry.getPool<core::Selectable>().getEntities();

    ImGui::Begin(Constants::ENTITIES.c_str());

    if (ImGui::Button("Add Entity")) {
      core::Entity newEntity = registry.createEntity();
      registry.addComponent<core::Selectable>(newEntity);
    }
    for (unsigned int i = 0;
         i < registry.getPool<core::Selectable>().getNumElements(); i++) {
      const core::Entity &entity = selectableEntities[i];
      std::string         str = Constants::ENTITY + std::to_string(entity);

      if (ImGui::Selectable(str.c_str(), selectedEntities.contains(entity))) {
        for (core::Entity e : selectedEntities.getEntities()) {
          selectedEntities.removeElem(e);
        }
        selectedEntities.addElem(entity);
      }
      if (ImGui::BeginPopupContextItem()) {
        ImGui::PushID(str.c_str());
        if (ImGui::Button("Delete")) {
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