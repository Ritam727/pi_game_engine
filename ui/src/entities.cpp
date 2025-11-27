#include "entities.hpp"

#include "imgui.h"
#include "selectable.hpp"

namespace ui {
  void Entities::entitiesWindow(core::SparseSet<core::Entity> &selectedEntities,
                                core::Registry                &registry) {
    std::vector<core::Entity> &selectableEntities =
        registry.getPool<core::Selectable>().getEntities();

    ImGui::Begin("Entities");

    for (unsigned int i = 0;
         i < registry.getPool<core::Selectable>().getNumElements(); i++) {
      core::Entity &entity = selectableEntities[i];
      std::string   str = "Entity " + std::to_string(entity);

      bool &isEntitySelected =
          registry.getPool<core::Selectable>().get(entity).selected;

      ImGui::Checkbox(str.c_str(), &isEntitySelected);

      if (isEntitySelected)
        selectedEntities.addElem(entity);
      else
        selectedEntities.removeElem(entity);
    }
    ImGui::End();
  }
}