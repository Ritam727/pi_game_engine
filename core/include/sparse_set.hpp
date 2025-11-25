#pragma once

#include "base_component.hpp"
#include "utils.hpp"
#include <vector>

namespace core {
  template <typename I> class SparseSet {
  protected:
    std::vector<I>            entities{};
    std::vector<unsigned int> sparse{};
    unsigned int              n{0};

  public:
    virtual ~SparseSet() = default;

    virtual bool contains(I e) {
      return e < this->n && this->sparse[e] < this->n && this->sparse[e] >= 0 &&
             e == this->entities[this->sparse[e]];
    }

    virtual void removeElem(I e) {
      if (!this->contains(e))
        return;
      unsigned int entityIndex = this->sparse[e];
      I            lastEntity = this->entities.back();
      unsigned int lastIndex = this->sparse[lastEntity];

      this->entities[entityIndex] = lastEntity;
      this->sparse[lastEntity] = entityIndex;
      this->n--;
    }
  };

  template <typename I, IsSubClassOf<BaseComponent> T>
  class ExtendedSparseSet : public SparseSet<I> {
  private:
    std::vector<T> components{};

  public:
    ExtendedSparseSet() {}

    ~ExtendedSparseSet() {
      for (T &component : components)
        component.clearComponent();
    }

    ExtendedSparseSet(unsigned int size) {
      this->entities.reserve(size);
      this->sparse.reserve(size);
      this->components.reserve(size);
    }

    template <typename... Args> void addElem(I entity, Args &&...args) {
      if (this->contains(entity))
        return;
      this->entities.emplace_back(entity);
      this->components.emplace_back(std::forward<Args>(args)...);
      if (this->sparse.size() <= entity)
        this->sparse.resize(entity + 1);
      this->sparse[entity] = this->n++;
    }

    void removeElem(I entity) override {
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