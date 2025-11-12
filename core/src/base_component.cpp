#include "base_component.hpp"

namespace core {
  BaseComponent::BaseComponent(ecs::Registry &registryRef)
      : registryRef(registryRef) {}

  ecs::Registry &BaseComponent::getRegistryRef() {
    return this->registryRef;
  }
}