#pragma once

#include "layer.hpp"
#include "window.hpp"

namespace ui {
  class UI : public core::Layer {
  private:
    core::Window &window;

  public:
    UI(core::Window &window);
    ~UI();

    void onUpdate(float ts) override;
    void postUpdate() override;
  };
}