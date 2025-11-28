#pragma once

#include "registry.hpp"
#include "sparse_set.hpp"

namespace ui {
  class UILayer {
  public:
    virtual ~UILayer() = default;

    virtual void onUpdate(float                          ts,
                          core::SparseSet<core::Entity> &selectedEntities,
                          core::Registry                &registry) = 0;
  };
}