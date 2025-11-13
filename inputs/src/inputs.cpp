#include "inputs.hpp"
#include "GLFW/glfw3.h"
#include "camera_transform.hpp"
#include "event_manager.hpp"
#include "events.hpp"
#include "constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace inputs {
  Inputs::Inputs(core::Window &window, core::Registry &registry)
      : window(window), registry(registry) {
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::KEY_EVENT, Inputs::keyPressHandler);
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::MOUSE_MOVEMENT_EVENT,
        Inputs::mouseMovementHandler);
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::MOUSE_SCROLL_EVENT, Inputs::mouseScrollHandler);
  }

  std::vector<int> &Inputs::getKeyPressStates() {
    static std::vector<int> keyPressStates(500, 0);
    return keyPressStates;
  }

  std::vector<int> &Inputs::getMouseButtonStates() {
    static std::vector<int> mouseButtonStates(5, 0);
    return mouseButtonStates;
  }

  MouseState &Inputs::getMousePositions() {
    static glm::vec2  mousePosition(0);
    static glm::vec2  prevMousePosition(0);
    static MouseState mousePositions{mousePosition, prevMousePosition, 0};
    return mousePositions;
  }

  ScreenFov &Inputs::getScreenFov() {
    static ScreenFov fov{45.0f, 0.0f};
    return fov;
  }

  void Inputs::keyPressHandler(core::InputEvent &event) {
    core::KeyEvent keyEvent = std::get<core::KeyEvent>(event.getData());
    if (keyEvent.getType() != core::InputAction::RELEASE) {
      Inputs::getKeyPressStates()[keyEvent.getKey()] = 1;
    } else {
      Inputs::getKeyPressStates()[keyEvent.getKey()] = 0;
    }
  }

  void Inputs::mouseMovementHandler(core::InputEvent &event) {
    core::MouseMovementEvent mouseMovementEvent =
        std::get<core::MouseMovementEvent>(event.getData());
    MouseState &mouseState = Inputs::getMousePositions();
    if (!mouseState.initialised) {
      mouseState.initialised = 1;
      mouseState.previousPosition.x = mouseMovementEvent.getX();
      mouseState.previousPosition.y = mouseMovementEvent.getY();
    }
    mouseState.currentPosition.x = mouseMovementEvent.getX();
    mouseState.currentPosition.y = mouseMovementEvent.getY();
  }

  void Inputs::mouseScrollHandler(core::InputEvent &event) {
    core::MouseScrollEvent mouseScrollEvent =
        std::get<core::MouseScrollEvent>(event.getData());
    ScreenFov &screenFov = Inputs::getScreenFov();
    screenFov.delta = mouseScrollEvent.getY();
  }

  void Inputs::onUpdate(float ts) {
    this->toggleCursorVisibility();
    this->updateFov(ts);
    this->updateCamera(ts);
  }

  void Inputs::updateFov(float ts) {
    Inputs::getScreenFov().fov +=
        Inputs::getScreenFov().delta * ts * core::Constants::SPEED_SCALAR;
  }

  void Inputs::toggleCursorVisibility() {
    std::vector<int> &keyStates = Inputs::getKeyPressStates();
    if ((keyStates[GLFW_KEY_LEFT_CONTROL] ||
         keyStates[GLFW_KEY_RIGHT_CONTROL]) &&
        (keyStates[GLFW_KEY_LEFT_SHIFT] || keyStates[GLFW_KEY_RIGHT_SHIFT])) {
      this->window.setCursorVisibility(false);
    } else {
      this->window.setCursorVisibility(true);
    }
  }

  void Inputs::updateCamera(float ts) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        glm::vec2 &currentPosition =
            Inputs::getMousePositions().currentPosition;
        glm::vec2 &previousPosition =
            Inputs::getMousePositions().previousPosition;

        float xOffset = (currentPosition.x - previousPosition.x) * ts *
                        core::Constants::SPEED_SCALAR;
        float yOffset = (previousPosition.y - currentPosition.y) * ts *
                        core::Constants::SPEED_SCALAR;

        cameraTransform.updateRotation({xOffset, yOffset});
        previousPosition = currentPosition;

        glm::vec3 direction(0);
        float     speed = 0.0f;
        if (Inputs::getKeyPressStates()[GLFW_KEY_W]) {
          direction += cameraTransform.getForwardDirection();
          speed = core::Constants::SPEED_SCALAR * 0.1;
        }
        if (Inputs::getKeyPressStates()[GLFW_KEY_S]) {
          direction -= cameraTransform.getForwardDirection();
          speed = core::Constants::SPEED_SCALAR * 0.1;
        }
        if (Inputs::getKeyPressStates()[GLFW_KEY_A]) {
          direction -= cameraTransform.getRightDirection();
          speed = core::Constants::SPEED_SCALAR * 0.1;
        }
        if (Inputs::getKeyPressStates()[GLFW_KEY_D]) {
          direction += cameraTransform.getRightDirection();
          speed = core::Constants::SPEED_SCALAR * 0.1;
        }

        cameraTransform.updatePosition(direction * ts * speed);
      }
    }
  }
}