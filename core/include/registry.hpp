#pragma once

#include "sparse_set.hpp"

#include <typeindex>
#include <unordered_map>
#include <memory>

namespace core {
  typedef unsigned int Entity;

  class Registry {
  private:
    std::unordered_map<std::type_index, std::unique_ptr<core::ISparseSet>>
           pools;
    Entity lastEntity = -1;

  public:
    Registry();

    void addEntity();

    Entity getLastEntity();
    Entity createEntity();

    template <typename T> void addComponent(Entity entity, T component) {
      std::type_index poolIndex = std::type_index(typeid(T));
      if (!this->pools.contains(poolIndex)) {
        this->pools[poolIndex] = std::make_unique<core::SparseSet<Entity, T>>();
      }
      static_cast<core::SparseSet<Entity, T> *>(this->pools[poolIndex].get())
          ->addElem(entity, component);
    }

    template <typename T> void removeComponent(Entity entity, T &component) {
      std::type_index poolIndex = std::type_index(typeid(T));
      static_cast<core::SparseSet<Entity, T> *>(this->pools[poolIndex].get())
          ->removeElem(entity);
    }

    template <typename T> core::SparseSet<Entity, T> &getPool() {
      std::type_index poolIndex = std::type_index(typeid(T));
      return *static_cast<core::SparseSet<Entity, T> *>(
          this->pools[poolIndex].get());
    }
  };
}