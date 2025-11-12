#pragma once

#include "registry.hpp"
#include "sparse_set.hpp"

namespace core {
  class BaseObject {
  private:
    core::Registry &registryRef;
    core::Entity    entityId;

  public:
    BaseObject(core::Registry &registryRef);

    core::Entity getEntityId();
  };
}