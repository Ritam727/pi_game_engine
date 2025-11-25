#include "registry.hpp"

namespace core {
  Registry::Registry() {}

  void Registry::addEntity() {
    this->entities.addElem(++this->lastEntity);
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
      this->entities.addElem(entity);
      this->poppedEntities.pop_back();
      return entity;
    }
  }

  void Registry::removeEntity(Entity entity) {
    this->entities.removeElem(entity);
    this->poppedEntities.emplace_back(entity);
    for (std::pair<const std::type_index, std::unique_ptr<SparseSet<Entity>>>
             &p : this->pools)
      p.second->removeElem(entity);
  }

  std::vector<Entity> &Registry::getAllEntities() {
    return this->entities.getEntities();
  }
}