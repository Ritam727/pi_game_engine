#pragma once

#include "registry.hpp"
#include "ui_layer.hpp"

namespace ui {
  class Entities : public UILayer {
  public:
    void onUpdate(float ts, core::SparseSet<core::Entity> &selectedEntities,
                  core::Registry &registry) override;

    static void entitiesWindow(core::SparseSet<core::Entity> &selectedEntities,
                               core::Registry                &registry);
  };
}