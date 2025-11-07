#include "base_object.hpp"

namespace core {
  BaseObject::BaseObject(ecs::Registry &registryRef)
      : registryRef(registryRef) {
    this->entityId = registryRef.createEntity();
  }

  ecs::Entity BaseObject::getEntityId() {
    return this->entityId;
  }
}