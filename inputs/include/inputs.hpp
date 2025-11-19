#pragma once

#include "layer.hpp"
#include "registry.hpp"
#include "state_manager.hpp"
#include "window.hpp"

namespace inputs {
  class Inputs : public core::Layer {
  private:
    core::Window   &window;
    core::Registry &registry;
    StateKeyMap    &stateManager;
    glm::vec2       mousePosition;

  public:
    Inputs(core::Window &window, core::Registry &registry,
           StateKeyMap &stateManager);

    void onUpdate(float ts) override;
    void updateCamera(float ts);
    void handleFovChange(float ts);
    void toggleCursorVisibility();
    void handleCameraStates();
  };
}