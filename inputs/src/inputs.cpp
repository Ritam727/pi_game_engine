#include "inputs.hpp"
#include "camera_transform.hpp"
#include "event_manager.hpp"
#include "events.hpp"
#include "constants.hpp"
#include "states.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace inputs {
  static InputState inputState{};

  bool areKeysPressed(std::vector<std::vector<unsigned int>> &keys) {
    bool isCombination = true;
    for (std::vector<unsigned int> &currentCombination : keys) {
      bool keysPressed = false;
      for (unsigned int &key : currentCombination) {
        keysPressed |=
            (inputState.keyboardState[key] != core::InputAction::RELEASE);
      }
      isCombination &= keysPressed;
    }
    return isCombination;
  }

  void keyPressHandler(core::InputEvent &event) {
    core::KeyEvent keyEvent = std::get<core::KeyEvent>(event.data);
    inputState.keyboardState[keyEvent.key] = keyEvent.type;
  }

  void mouseMovementHandler(core::InputEvent &event) {
    core::MouseMovementEvent mouseMovementEvent =
        std::get<core::MouseMovementEvent>(event.data);
    if (inputState.resetMousePosition == MousePositionReset::INIT) {
      inputState.resetMousePosition = MousePositionReset::DONE;
    }
    inputState.mousePosition =
        glm::vec2(mouseMovementEvent.x, mouseMovementEvent.y);
  }

  void mouseScrollHandler(core::InputEvent &event) {
    core::MouseScrollEvent mouseScrollEvent =
        std::get<core::MouseScrollEvent>(event.data);
    inputState.scrollDelta = mouseScrollEvent.y;
  }
}

namespace inputs {
  Inputs::Inputs(core::Window &window, core::Registry &registry,
                 StateKeyMap &stateManager)
      : window(window), registry(registry), stateManager(stateManager) {
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::KEY_EVENT, keyPressHandler);
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::MOUSE_MOVEMENT_EVENT, mouseMovementHandler);
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::MOUSE_SCROLL_EVENT, mouseScrollHandler);
  }

  void Inputs::onUpdate(float ts) {
    this->handleCameraStates();
    this->toggleCursorVisibility();
    this->updateCamera(ts);
  }

  void Inputs::handleCameraStates() {
    StateType type = StateType::CAMERA_STATE;
    std::unordered_map<unsigned int, std::vector<std::vector<unsigned int>>>
        &inputMap = this->stateManager.getInputMap(type);
    for (std::pair<const unsigned int, std::vector<std::vector<unsigned int>>>
             &p : inputMap) {
      if (areKeysPressed(p.second)) {
        inputState.cameraState = static_cast<CameraState>(p.first);
        return;
      }
    }
    inputState.cameraState = CameraState::DEFAULT;
  }

  void Inputs::toggleCursorVisibility() {
    CameraState cameraState = inputState.cameraState;
    if (cameraState == CameraState::CAMERA_FLY_MODE ||
        cameraState == CameraState::CAMERA_PAN_MODE) {
      this->window.setCursorVisibility(false);
    } else {
      this->window.setCursorVisibility(true);
      inputState.resetMousePosition = MousePositionReset::INIT;
    }
  }

  void Inputs::updateCamera(float ts) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        if (inputState.cameraState == CameraState::CAMERA_RESET_MODE) {
          cameraTransform.resetCameraTransform();
          break;
        }
        if (inputState.resetMousePosition == MousePositionReset::DONE) {
          this->mousePosition = inputState.mousePosition;
          inputState.resetMousePosition = MousePositionReset::NONE;
        }
        glm::vec2 &currentPosition = inputState.mousePosition;
        glm::vec2 &previousPosition = this->mousePosition;

        float xOffset = (currentPosition.x - previousPosition.x) * ts *
                        core::Constants::SPEED_SCALAR * 0.2;
        float yOffset = (previousPosition.y - currentPosition.y) * ts *
                        core::Constants::SPEED_SCALAR * 0.2;
        float &scrollDelta = inputState.scrollDelta;
        previousPosition = currentPosition;

        CameraState cameraState = inputState.cameraState;
        glm::vec3   direction(0);
        float       speed = 0.0f;

        if (cameraState == CameraState::CAMERA_PAN_MODE) {
          cameraTransform.updateRotation({xOffset, yOffset});
        } else if (cameraState == CameraState::CAMERA_FLY_MODE) {
          glm::vec3 front = cameraTransform.getForwardDirection();
          glm::vec3 right = cameraTransform.getRightDirection();
          glm::vec3 up = glm::normalize(glm::cross(right, front));
          if (xOffset != 0 || yOffset != 0) {
            direction = glm::normalize(xOffset * right + yOffset * up);
            speed = core::Constants::SPEED_SCALAR * 0.1;
          } else if (scrollDelta != 0) {
            direction = -1.0f * scrollDelta * glm::normalize(front);
            scrollDelta = 0;
            speed = core::Constants::SPEED_SCALAR;
          }
          cameraTransform.updatePosition(direction * ts * speed);
        }
      }
    }
  }
}