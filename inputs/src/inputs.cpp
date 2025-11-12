#include "inputs.hpp"
#include "GLFW/glfw3.h"
#include "camera_transform.hpp"
#include "event_manager.hpp"
#include "events.hpp"
#include "constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace inputs {
  Inputs::Inputs(core::Registry &registry) : registry(registry) {
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::KEY_EVENT, Inputs::keyPressCallback);
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::MOUSE_MOVEMENT_EVENT,
        Inputs::mouseMovementCallback);
  }

  std::vector<int> &Inputs::getKeyPressStates() {
    static std::vector<int> keyPressStates(500, 0);
    return keyPressStates;
  }

  std::vector<int> &Inputs::getMouseButtonStates() {
    static std::vector<int> mouseButtonStates(5, 0);
    return mouseButtonStates;
  }

  std::tuple<glm::vec2, glm::vec2, float> &Inputs::getMousePositions() {
    static glm::vec2                               mousePosition(0);
    static glm::vec2                               prevMousePosition(0);
    static std::tuple<glm::vec2, glm::vec2, float> mousePositions = {
        mousePosition, prevMousePosition, 0};
    return mousePositions;
  }

  void Inputs::keyPressCallback(core::InputEvent &event) {
    core::KeyEvent keyEvent = std::get<core::KeyEvent>(event.getData());
    if (keyEvent.getType() != core::InputAction::RELEASE) {
      Inputs::getKeyPressStates()[keyEvent.getKey()] = 1;
    } else {
      Inputs::getKeyPressStates()[keyEvent.getKey()] = 0;
    }
  }

  void Inputs::mouseMovementCallback(core::InputEvent &event) {
    core::MouseMovementEvent mouseMovementEvent =
        std::get<core::MouseMovementEvent>(event.getData());
    std::tuple<glm::vec2, glm::vec2, float> &mouseInfo =
        Inputs::getMousePositions();
    if (std::get<2>(mouseInfo) == 0) {
      std::get<2>(mouseInfo) = 1;
      std::get<1>(mouseInfo).x = mouseMovementEvent.getX();
      std::get<1>(mouseInfo).y = mouseMovementEvent.getY();
    }
    std::get<0>(mouseInfo).x = mouseMovementEvent.getX();
    std::get<0>(mouseInfo).y = mouseMovementEvent.getY();
  }

  void Inputs::onUpdate(float ts) {
    this->updateCamera(ts);
  }

  void Inputs::updateCamera(float ts) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        glm::vec2 &currentPosition = std::get<0>(Inputs::getMousePositions());
        glm::vec2 &previousPosition = std::get<1>(Inputs::getMousePositions());

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