#pragma once

#include <vector>

namespace commons {
  class ISparseSet {
  public:
    virtual ~ISparseSet() = default;
  };

  template <typename I, typename T> class SparseSet : public ISparseSet {
  private:
    std::vector<I>            entities;
    std::vector<unsigned int> sparse;
    std::vector<T>            components;

    unsigned int n = 0;

  public:
    SparseSet() {}

    SparseSet(unsigned int size) {
      this->entities.reserve(size);
      this->sparse.reserve(size);
      this->components.reserve(size);
    }

    void addElem(I entity, T component) {
      if (this->contains(entity))
        return;
      this->entities.emplace_back(entity);
      this->components.emplace_back(component);
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
  };
}