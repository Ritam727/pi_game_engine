#pragma once

#include "registry.hpp"

namespace gl {
  class BaseComponent {
  private:
    ecs::Registry &registryRef;

  public:
    BaseComponent(ecs::Registry &registryRef);

    ecs::Registry &getRegistryRef();
  };
}