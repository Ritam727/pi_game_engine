#pragma once

#include "registry.hpp"

namespace core {
  class BaseComponent {
  private:
    core::Registry &registryRef;

  public:
    BaseComponent(core::Registry &registryRef);

    core::Registry &getRegistryRef();
  };
}