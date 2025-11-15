#pragma once

#include "events.hpp"
#include "layer.hpp"
#include "registry.hpp"
#include "state_manager.hpp"
#include "window.hpp"

#include <vector>

namespace inputs {
  struct MouseState {
    glm::vec2 currentPosition;
    glm::vec2 previousPosition;
    bool      initialised;
  };

  struct ScreenFov {
    float fov;
    float delta;
  };
}

namespace inputs {
  class Inputs : public core::Layer {
  private:
    core::Window   &window;
    core::Registry &registry;
    StateManager   &stateManager;

  public:
    Inputs(core::Window &window, core::Registry &registry,
           StateManager &stateManager);

    void onUpdate(float ts) override;
    void updateCamera(float ts);
    void toggleCursorVisibility();
    void handleCameraStates();

    static bool areKeysPressed(std::vector<std::vector<unsigned int>> &keys);

    static std::vector<int> &getKeyPressStates();
    static std::vector<int> &getMouseButtonStates();
    static MouseState       &getMousePositions();
    static float            &getMouseScrollDelta();
    static float            &getScreenFov();

    static void keyPressHandler(core::InputEvent &event);
    static void mouseMovementHandler(core::InputEvent &event);
    static void mouseScrollHandler(core::InputEvent &event);
  };
}