#pragma once

#include "commons.hpp"
#include "registry.hpp"

namespace ui {
  class UILayer {
  protected:
    UIState &uiState;

  public:
    UILayer(UIState &uiState) : uiState(uiState) {}
    virtual ~UILayer() = default;

    virtual void onUpdate(float ts, core::Registry &registry) = 0;
  };
}