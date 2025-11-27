#pragma once

#include "registry.hpp"

namespace ui {
  class Components {
  public:
    static void
        componentsWindow(core::SparseSet<core::Entity> &selectedEntities,
                         core::Registry                &registry);
  };
}