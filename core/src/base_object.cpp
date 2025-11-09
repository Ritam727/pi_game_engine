#include "base_object.hpp"

namespace gl {
  BaseObject::BaseObject(ecs::Registry &registryRef)
      : registryRef(registryRef) {
    this->entityId = registryRef.createEntity();
  }

  ecs::Entity BaseObject::getEntityId() {
    return this->entityId;
  }
}