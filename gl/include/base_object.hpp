#pragma once

#include "registry.hpp"
#include "sparse_set.hpp"

namespace core {
  class BaseObject {
  private:
    ecs::Registry &registryRef;
    ecs::Entity    entityId;

  public:
    BaseObject(ecs::Registry &registryRef);

    ecs::Entity getEntityId();
  };
}