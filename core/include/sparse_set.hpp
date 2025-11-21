#pragma once

#include "base_component.hpp"
#include "utils.hpp"
#include <vector>

namespace core {
  class ISparseSet {
  public:
    virtual ~ISparseSet() = default;
  };

  template <typename I, IsSubClassOf<BaseComponent> T>
  class SparseSet : public ISparseSet {
  private:
    std::vector<I>            entities{};
    std::vector<unsigned int> sparse{};
    std::vector<T>            components{};

    unsigned int n = 0;

  public:
    SparseSet() {}

    ~SparseSet() {
      for (T &component : components)
        component.clearComponent();
    }

    SparseSet(unsigned int size) {
      this->entities.reserve(size);
      this->sparse.reserve(size);
      this->components.reserve(size);
    }

    template <typename... Args> void addElem(I entity, Args... args) {
      if (this->contains(entity))
        return;
      this->entities.emplace_back(entity);
      this->components.emplace_back(std::forward<Args>(args)...);
      if (this->sparse.size() <= entity)
        this->sparse.resize(entity + 1);
      this->sparse[entity] = this->n++;
    }

    void removeElem(I entity) {
      if (!this->contains(entity))
        return;
      unsigned int entityIndex = this->sparse[entity];
      I            lastEntity = this->entities.back();
      T           &lastComponent = this->components.back();
      unsigned int lastIndex = this->sparse[lastEntity];

      this->entities[entityIndex] = lastEntity;
      this->components[entityIndex] = lastComponent;
      this->sparse[lastEntity] = entityIndex;
      this->n--;
    }

    bool contains(I e) {
      return e < this->n && this->sparse[e] < this->n && this->sparse[e] >= 0 &&
             e == this->entities[this->sparse[e]];
    }

    T &get(I e) {
      return this->components[this->sparse[e]];
    }

    std::vector<T> &getComponents() {
      return this->components;
    }

    std::vector<I> &getEntities() {
      return this->entities;
    }
  };
}