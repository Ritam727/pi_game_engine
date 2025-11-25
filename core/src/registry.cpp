#include "registry.hpp"

namespace core {
  Registry::Registry() {}

  void Registry::addEntity() {
    this->lastEntity++;
  }

  Entity Registry::getLastEntity() {
    return this->lastEntity;
  }

  Entity Registry::createEntity() {
    if (this->poppedEntities.empty()) {
      this->addEntity();
      return this->getLastEntity();
    } else {
      Entity entity = this->poppedEntities.back();
      this->poppedEntities.pop_back();
      return entity;
    }
  }

  void Registry::removeEntity(Entity entity) {
    this->poppedEntities.emplace_back(entity);
    for (std::pair<const std::type_index, std::unique_ptr<SparseSet<Entity>>>
             &p : this->pools)
      p.second->removeElem(entity);
  }
}