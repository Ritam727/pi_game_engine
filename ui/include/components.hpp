#pragma once

#include "base_component.hpp"
#include "commons.hpp"
#include "imgui.h"
#include "registry.hpp"
#include "ui_layer.hpp"
#include "utils.hpp"

namespace ui {
  class Components : public UILayer {
  private:
    void componentsWindow(core::Registry &registry);

    template <core::IsSubClassOf<core::BaseComponent> T, typename... Args>
    static void componentControls(core::Registry       &registry,
                                  const std::string    &header,
                                  const core::Entity   &entity,
                                  std::function<void()> controls) {
      if (registry.getPool<T>().contains(entity)) {
        unsigned int flags =
            ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable;
        bool isDeleted{false};

        ImGui::BeginTable(typeid(T).name(), 2, flags);
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", header.c_str());

        ImGui::TableSetColumnIndex(1);
        ImGui::PushID(typeid(T).name());
        if (ImGui::Button(Constants::DELETE.c_str())) {
          registry.removeComponent<T>(entity);
          isDeleted = true;
        }
        ImGui::PopID();

        ImGui::EndTable();

        if (isDeleted)
          return;
        controls();
      }
    }

  public:
    Components(UIState &uiState);

    void onUpdate(float ts, core::Registry &registry) override;
  };
}