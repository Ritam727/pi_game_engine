#pragma once

#include "layer.hpp"
#include "registry.hpp"
#include "state_manager.hpp"
#include "states.hpp"
#include "window.hpp"

namespace inputs {
  class Inputs : public core::Layer {
  private:
    core::Window       &window;
    core::Registry     &registry;
    core::EventManager &eventManager;
    StateManager        stateManager{};
    State               inputState{};
    glm::vec2           mousePosition;

    void registerKeyCallback();
    void registerMouseButtonCallback();
    void registerMouseMoveCallback();
    void registerMouseScrollCallback();
    void cameraMoveAround(float xOffset, float yOffset);
    void cameraPan(float xOffset, float yOffset);
    void cameraFly(float delta);

  public:
    Inputs(core::Window &window, core::Registry &registry,
           core::EventManager &eventManager);

    void onUpdate(float ts) override;
    void handleFovChange();
    void toggleCursorVisibility();
  };
}