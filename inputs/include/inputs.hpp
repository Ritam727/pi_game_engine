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

/*
Maintain a list of currently pressed keys, which is appended everytime a key is
pressed and an entry is removed everytime a key is removed. That is going to
give us the current sequence of key presses. Ensure that buttons are not
repeated.

Algorithm:
1. Maintain a pointer to the first location in the list of key presses which
indicates the end of the activations that have been fired.
2. Accept a keypress only when the pointer is at the beginning of the list.
3. Reset the pointer only when the list becomes empty, ie, all keypresses have
been removed.

How to determine whether to fire an activation?
1. Manually iterate over all activations to find whether the current combination
is has an activation corresponding to it. This is slow (O(n * k) with some
optimizations, since we have to iterate over all prefixes and match these
prefixes with the key combinations of the shortcuts. Matching itself can be
optimized using hashing.). We need a way to efficiently check whether an
activation is fired solely based on the prefix.
2. Instead of iterating over all the shortcuts, save a map with the key as an
encoding over the combination (can be as simple as underscore or plus separated
string) and value as the function to call. This would give us a complexity of
O(n) every frame where n < 500, which should be pretty fast given our use case.
Each activation would have an onPress and onRelease which would be fired
depending on whether the activation was effectively "fired" or "to exit" from
the activation.


The above approach doesn't work well while handling prefixes.
*/