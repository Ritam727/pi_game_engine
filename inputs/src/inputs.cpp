#include "inputs.hpp"
#include "GLFW/glfw3.h"
#include "camera_transform.hpp"
#include "event_manager.hpp"
#include "events.hpp"
#include "constants.hpp"
#include "states.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace inputs {
  Inputs::Inputs(core::Window &window, core::Registry &registry,
                 StateManager &stateManager)
      : window(window), registry(registry), stateManager(stateManager) {
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
    static ScreenFov screenFov{45.0f, 0.0f};
    return screenFov;
  }

  void Inputs::keyPressHandler(core::InputEvent &event) {
    core::KeyEvent keyEvent = std::get<core::KeyEvent>(event.data);
    if (keyEvent.type != core::InputAction::RELEASE) {
      Inputs::getKeyPressStates()[keyEvent.key] = 1;
    } else {
      Inputs::getKeyPressStates()[keyEvent.key] = 0;
    }
  }

  void Inputs::mouseMovementHandler(core::InputEvent &event) {
    core::MouseMovementEvent mouseMovementEvent =
        std::get<core::MouseMovementEvent>(event.data);
    MouseState &mouseState = Inputs::getMousePositions();
    if (!mouseState.initialised) {
      mouseState.initialised = 1;
      mouseState.previousPosition.x = mouseMovementEvent.x;
      mouseState.previousPosition.y = mouseMovementEvent.y;
    }
    mouseState.currentPosition.x = mouseMovementEvent.x;
    mouseState.currentPosition.y = mouseMovementEvent.y;
  }

  void Inputs::mouseScrollHandler(core::InputEvent &event) {
    core::MouseScrollEvent mouseScrollEvent =
        std::get<core::MouseScrollEvent>(event.data);
    ScreenFov &screenFov = Inputs::getScreenFov();
    screenFov.delta = mouseScrollEvent.y;
  }

  bool Inputs::areKeysPressed(std::vector<std::vector<unsigned int>> &keys) {
    bool isCombination = true;
    for (std::vector<unsigned int> &currentCombination : keys) {
      bool keysPressed = false;
      for (unsigned int &key : currentCombination) {
        keysPressed |= Inputs::getKeyPressStates()[key];
      }
      isCombination &= keysPressed;
    }
    return isCombination;
  }

  void Inputs::onUpdate(float ts) {
    this->handleCameraStates();
    this->toggleCursorVisibility();
    this->updateFov(ts);
    this->updateCamera(ts);
  }

  void Inputs::handleCameraStates() {
    StateType type = StateType::CAMERA_STATE;
    State    &currentState = this->stateManager.getState();
    std::unordered_map<unsigned int, std::vector<std::vector<unsigned int>>>
        &inputMap = this->stateManager.getInputMap(type);
    for (std::pair<const unsigned int, std::vector<std::vector<unsigned int>>>
             &p : inputMap) {
      if (Inputs::areKeysPressed(p.second)) {
        currentState.cameraState = static_cast<CameraState>(p.first);
        return;
      }
    }
    currentState.cameraState = CameraState::DEFAULT;
  }

  void Inputs::updateFov(float ts) {
    Inputs::getScreenFov().fov +=
        Inputs::getScreenFov().delta * ts * core::Constants::SPEED_SCALAR;
  }

  void Inputs::toggleCursorVisibility() {
    std::vector<int> &keyStates = Inputs::getKeyPressStates();
    CameraState       cameraState = this->stateManager.getState().cameraState;
    if (cameraState == CameraState::CAMERA_FLY_MODE ||
        cameraState == CameraState::CAMERA_PAN_MODE) {
      this->window.setCursorVisibility(false);
    } else {
      this->window.setCursorVisibility(true);
      Inputs::getMousePositions().initialised = 0;
    }
  }

  void Inputs::updateCamera(float ts) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        if (this->stateManager.getState().cameraState ==
            CameraState::CAMERA_RESET_MODE) {
          cameraTransform.resetCameraTransform();
          break;
        }
        glm::vec2 &currentPosition =
            Inputs::getMousePositions().currentPosition;
        glm::vec2 &previousPosition =
            Inputs::getMousePositions().previousPosition;

        float xOffset = (currentPosition.x - previousPosition.x) * ts *
                        core::Constants::SPEED_SCALAR;
        float yOffset = (previousPosition.y - currentPosition.y) * ts *
                        core::Constants::SPEED_SCALAR;
        previousPosition = currentPosition;

        CameraState cameraState = this->stateManager.getState().cameraState;
        glm::vec3   direction(0);
        float       speed = 0.0f;

        if (cameraState == CameraState::CAMERA_PAN_MODE) {
          cameraTransform.updateRotation({xOffset, yOffset});
        } else if (cameraState == CameraState::CAMERA_FLY_MODE) {
          glm::vec3 front = cameraTransform.getForwardDirection();
          glm::vec3 right = cameraTransform.getRightDirection();
          glm::vec3 up = glm::normalize(glm::cross(right, front));
          direction = glm::normalize(xOffset * right + yOffset * up);
          speed = core::Constants::SPEED_SCALAR * 0.1;
          cameraTransform.updatePosition(direction * ts * speed);
        }
      }
    }
  }
}