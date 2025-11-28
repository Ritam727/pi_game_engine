#pragma once

#include "layer.hpp"
#include "layer_stack.hpp"
#include "registry.hpp"
#include "ui_layer.hpp"
#include "utils.hpp"
#include "window.hpp"
#include <memory>
#include <vector>

namespace ui {
  class UI : public core::Layer {
  private:
    core::Window                 &window;
    core::Registry               &registry;
    core::SparseSet<core::Entity> selectedEntities{};
    core::LayerStack<UILayer>     uiLayers;

  public:
    UI(core::Window &window, core::Registry &registry);
    ~UI();

    void onUpdate(float ts) override;
    void postUpdate() override;

    static void createDockspace();
  };
}