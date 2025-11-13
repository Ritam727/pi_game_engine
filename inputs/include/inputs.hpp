#pragma once

#include "events.hpp"
#include "layer.hpp"
#include "registry.hpp"
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

  public:
    Inputs(core::Window &window, core::Registry &registry);

    void onUpdate(float ts) override;
    void updateCamera(float ts);
    void updateFov(float ts);
    void toggleCursorVisibility();

    static std::vector<int> &getKeyPressStates();
    static std::vector<int> &getMouseButtonStates();
    static MouseState       &getMousePositions();
    static ScreenFov        &getScreenFov();

    static void keyPressHandler(core::InputEvent &event);
    static void mouseMovementHandler(core::InputEvent &event);
    static void mouseScrollHandler(core::InputEvent &event);
  };
}