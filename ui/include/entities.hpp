#pragma once

#include "registry.hpp"
#include "ui_layer.hpp"

namespace ui {
  class Entities : public UILayer {
  public:
    Entities(UIState &uiState);

    void onUpdate(float ts, core::Registry &registry) override;

    static void entitiesWindow(core::Entity   &selectedEntity,
                               core::Registry &registry);
  };
}