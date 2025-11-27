#pragma once

#include "layer.hpp"
#include "registry.hpp"
#include "window.hpp"

namespace ui {
  class UI : public core::Layer {
  private:
    core::Window                 &window;
    core::Registry               &registry;
    core::SparseSet<core::Entity> selectedEntities{};

  public:
    UI(core::Window &window, core::Registry &registry);
    ~UI();

    void onUpdate(float ts) override;
    void postUpdate() override;

    static void createDockspace();
  };
}