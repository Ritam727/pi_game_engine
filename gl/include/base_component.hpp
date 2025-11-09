#pragma once

#include "registry.hpp"

namespace core {
  class BaseComponent {
  private:
    ecs::Registry &registryRef;

  public:
    BaseComponent(ecs::Registry &registryRef);

    ecs::Registry &getRegistryRef();
  };
}