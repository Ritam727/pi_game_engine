#pragma once

#include "registry.hpp"

namespace core {
  class BaseObject {
  private:
    core::Entity entityId;

  public:
    BaseObject(core::Registry &registryRef);

    core::Entity getEntityId();
  };
}