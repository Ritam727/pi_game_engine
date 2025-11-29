#pragma once

#include "base_component.hpp"
#include "utils.hpp"
#include <vector>

namespace core {
  template <typename I> class SparseSet {
  protected:
    std::vector<I>            entities{};
    std::vector<unsigned int> sparse{};
    unsigned int              u{0};
    unsigned int              n{0};

  public:
    virtual ~SparseSet() = default;

    unsigned int getNumElements() const {
      return this->n;
    }

    virtual void addElem(I e) {
      if (this->contains(e))
        return;
      this->n++;
      if (this->entities.size() < this->n)
        this->entities.resize(this->n);
      this->entities[this->n - 1] = e;
      if (this->sparse.size() <= e) {
        this->u = e + 1;
        this->sparse.resize(this->u);
      }
      this->sparse[e] = this->n - 1;
    }

    bool contains(I e) const {
      return e < this->u && this->sparse[e] < this->n && this->sparse[e] >= 0 &&
             e == this->entities[this->sparse[e]];
    }

    virtual void removeElem(I e) {
      if (!this->contains(e))
        return;
      unsigned int entityIndex = this->sparse[e];
      I            lastEntity = this->entities[this->n - 1];
      unsigned int lastIndex = this->sparse[lastEntity];

      this->entities[entityIndex] = lastEntity;
      this->sparse[lastEntity] = entityIndex;
      this->entities[lastIndex] = this->u;
      this->n--;
    }

    const std::vector<I> &getEntities() const {
      return this->entities;
    }

    bool intersects(const SparseSet<I> &other) const {
      if (other.getNumElements() < this->n) {
        const std::vector<I> &entities = other.getEntities();
        for (unsigned int i = 0; i < other.getNumElements(); i++) {
          if (this->contains(entities[i]))
            return true;
        }
        return false;
      } else {
        for (unsigned int i = 0; i < this->n; i++) {
          if (other.contains(this->entities[i]))
            return true;
        }
        return false;
      }
    }
  };
}

namespace core {
  template <typename I, IsSubClassOf<BaseComponent> T>
  class ExtendedSparseSet : public SparseSet<I> {
  private:
    std::vector<T> components{};

  public:
    ~ExtendedSparseSet() {
      for (T &component : components)
        component.clearComponent();
    }

    template <typename... Args> void addComponent(I entity, Args &&...args) {
      SparseSet<I>::addElem(entity);
      if (this->components.size() < this->n)
        this->components.resize(this->n);
      this->components.emplace(this->components.begin() + this->n - 1,
                               std::forward<Args>(args)...);
    }

    void removeElem(I entity) override {
      if (!this->contains(entity))
        return;
      unsigned int entityIndex = this->sparse[entity];
      I            lastEntity = this->entities[this->n - 1];
      T           &lastComponent = this->components[this->n - 1];
      unsigned int lastIndex = this->sparse[lastEntity];

      this->entities[entityIndex] = lastEntity;
      this->components[entityIndex] = lastComponent;
      this->sparse[lastEntity] = entityIndex;
      this->entities[lastIndex] = this->u;
      this->n--;

      this->entities.pop_back();
      this->components.pop_back();
    }

    T &get(I e) {
      return this->components[this->sparse[e]];
    }

    std::vector<T> &getComponents() {
      return this->components;
    }
  };
}