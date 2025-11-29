#pragma once

#include "commons.hpp"
#include "layer.hpp"
#include "layer_stack.hpp"
#include "registry.hpp"
#include "ui_layer.hpp"
#include "window.hpp"

namespace ui {
  class UI : public core::Layer {
  private:
    core::Window                 &window;
    core::Registry               &registry;
    core::SparseSet<core::Entity> selectedEntities{};
    core::LayerStack<UILayer>     uiLayers;
    UIState                       uiState{};

  public:
    UI(core::Window &window, core::Registry &registry);
    ~UI();

    void onUpdate(float ts) override;
    void postUpdate() override;

    static void createDockspace();
  };
}