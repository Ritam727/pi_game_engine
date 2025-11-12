#pragma once

#include "events.hpp"
#include "layer.hpp"
#include "registry.hpp"
#include "GLFW/glfw3.h"

#include <tuple>
#include <vector>

namespace inputs {
  class Inputs : public core::Layer {
  private:
    core::Registry &registry;

  public:
    Inputs(core::Registry &registry);

    void onUpdate(float ts) override;
    void updateCamera(float ts);

    static std::vector<int>                        &getKeyPressStates();
    static std::vector<int>                        &getMouseButtonStates();
    static std::tuple<glm::vec2, glm::vec2, float> &getMousePositions();

    static void keyPressCallback(core::InputEvent &event);
    static void mouseMovementCallback(core::InputEvent &event);
  };
}