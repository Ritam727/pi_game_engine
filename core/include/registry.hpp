#pragma once

#include "sparse_set.hpp"

#include <typeindex>
#include <unordered_map>
#include <memory>

namespace ecs {
  class Registry {
  private:
    std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> pools;
    Entity lastEntity = -1;

  public:
    Registry();

    void addEntity();

    Entity getLastEntity();
    Entity createEntity();

    template <typename T> void addComponent(Entity entity, T &component) {
      std::type_index poolIndex = std::type_index(typeid(T));
      if (!this->pools.contains(poolIndex)) {
        this->pools[poolIndex] = std::make_unique<SparseSet<T>>();
      }
      static_cast<SparseSet<T> *>(this->pools[poolIndex].get())
          ->addElem(entity, component);
    }

    template <typename T> void removeComponent(Entity entity, T &component) {
      std::type_index poolIndex = std::type_index(typeid(T));
      static_cast<SparseSet<T> *>(this->pools[poolIndex].get())
          ->removeElem(entity);
    }

    template <typename T> SparseSet<T> &getPool() {
      std::type_index poolIndex = std::type_index(typeid(T));
      return *static_cast<SparseSet<T> *>(this->pools[poolIndex].get());
    }
  };
}