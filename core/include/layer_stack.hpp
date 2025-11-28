#pragma once

#include "utils.hpp"

#include <memory>
#include <vector>
#include <ranges>

namespace core {
  template <typename T> class LayerStack {
  private:
    std::vector<std::unique_ptr<T>> layers;

  public:
    template <IsSubClassOf<T> S, typename... Args>
    void pushLayer(Args &&...args) {
      this->layers.emplace_back(
          std::make_unique<S>(std::forward<Args>(args)...));
    }

    template <typename... Args> void onUpdate(Args &&...args) {
      for (std::unique_ptr<T> &layer : std::ranges::reverse_view(this->layers))
        layer->onUpdate(std::forward<Args>(args)...);
    }

    template <typename... Args> void postUpdate(Args &&...args) {
      for (std::unique_ptr<T> &layer : std::ranges::reverse_view(this->layers))
        layer->postUpdate(std::forward<Args>(args)...);
    }
  };
}