#pragma once

#include "layer.hpp"
#include "registry.hpp"
#include "state_manager.hpp"
#include "states.hpp"
#include "window.hpp"

namespace inputs {
  class Inputs {
  private:
    core::Window       &window;
    core::Registry     &registry;
    core::EventManager &eventManager;
    StateManager        stateManager{};
    State               inputState{};
    std::vector<float>  fovMap{45.0f, 60.0f, 75.0f};

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
  };
}