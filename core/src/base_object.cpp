#include "base_object.hpp"

namespace core {
  BaseObject::BaseObject(core::Registry &registryRef) {
    this->entityId = registryRef.createEntity();
  }

  core::Entity BaseObject::getEntityId() {
    return this->entityId;
  }
}