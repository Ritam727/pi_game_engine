#pragma once

#include "commons.hpp"
#include "registry.hpp"
#include "ui_layer.hpp"

namespace ui {
  class Components : public UILayer {
  private:
    UIState &uiState;

    void componentsWindow(core::SparseSet<core::Entity> &selectedEntities,
                          core::Registry                &registry);

  public:
    Components(UIState &uiState);

    void onUpdate(float ts, core::SparseSet<core::Entity> &selectedEntities,
                  core::Registry &registry) override;
  };
}