#pragma once

#include "registry.hpp"

namespace core {
  class BaseObject {
  private:
    core::Entity entityId;

  public:
    BaseObject(core::Registry &registryRef);
    virtual ~BaseObject() = default;

    core::Entity getEntityId();
  };
}