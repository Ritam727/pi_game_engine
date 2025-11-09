#include "registry.hpp"

namespace ecs {
  Registry::Registry() {}

  void Registry::addEntity() {
    this->lastEntity++;
  }

  Entity Registry::getLastEntity() {
    return this->lastEntity;
  }

  Entity Registry::createEntity() {
    this->addEntity();
    return this->getLastEntity();
  }
}