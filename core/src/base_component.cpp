#include "base_component.hpp"

namespace gl {
  BaseComponent::BaseComponent(ecs::Registry &registryRef)
      : registryRef(registryRef) {}

  ecs::Registry &BaseComponent::getRegistryRef() {
    return this->registryRef;
  }
}