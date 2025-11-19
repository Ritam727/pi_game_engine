#pragma once

#include "layer.hpp"
#include "registry.hpp"
#include "states.hpp"
#include "window.hpp"

namespace inputs {
  class Inputs : public core::Layer {
  private:
    core::Window   &window;
    core::Registry &registry;
    glm::vec2       mousePosition;

  public:
    Inputs(core::Window &window, core::Registry &registry);

    void onUpdate(float ts) override;
    void updateCamera(float ts);
    void handleFovChange(float ts);
    void toggleCursorVisibility();
  };
}