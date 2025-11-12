#include "base_component.hpp"

namespace core {
  BaseComponent::BaseComponent(core::Registry &registryRef)
      : registryRef(registryRef) {}

  core::Registry &BaseComponent::getRegistryRef() {
    return this->registryRef;
  }
}