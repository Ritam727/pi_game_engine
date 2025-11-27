#pragma once

#include "registry.hpp"

namespace ui {
  class Entities {
  public:
    static void entitiesWindow(core::SparseSet<core::Entity> &selectedEntities,
                               core::Registry                &registry);
  };
}