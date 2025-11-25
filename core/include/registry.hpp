#pragma once

#include "sparse_set.hpp"

#include <typeindex>
#include <unordered_map>
#include <memory>

namespace core {
  typedef int Entity;

  class Registry {
  private:
    std::unordered_map<std::type_index,
                       std::unique_ptr<core::SparseSet<Entity>>>
                            pools{};
    core::SparseSet<Entity> entities;
    std::vector<Entity>     poppedEntities{};
    Entity                  lastEntity{-1};

  public:
    Registry();

    void addEntity();

    Entity getLastEntity();
    Entity createEntity();

    void removeEntity(Entity entity);

    std::vector<Entity> &getAllEntities();

    template <typename T, typename... Args>
    void addComponent(Entity entity, Args &&...args) {
      std::type_index poolIndex = std::type_index(typeid(T));
      if (!this->pools.contains(poolIndex)) {
        this->pools[poolIndex] =
            std::make_unique<core::ExtendedSparseSet<Entity, T>>();
      }
      static_cast<core::ExtendedSparseSet<Entity, T> *>(
          this->pools[poolIndex].get())
          ->addComponent(entity, std::forward<Args>(args)...);
    }

    template <typename T> void removeComponent(Entity entity, T &component) {
      std::type_index poolIndex = std::type_index(typeid(T));
      static_cast<core::ExtendedSparseSet<Entity, T> *>(
          this->pools[poolIndex].get())
          ->removeElem(entity);
    }

    template <typename T> core::ExtendedSparseSet<Entity, T> &getPool() {
      std::type_index poolIndex = std::type_index(typeid(T));
      if (!this->pools.contains(poolIndex)) {
        this->pools[poolIndex] =
            std::make_unique<core::ExtendedSparseSet<Entity, T>>();
      }
      return *static_cast<core::ExtendedSparseSet<Entity, T> *>(
          this->pools[poolIndex].get());
    }
  };
}