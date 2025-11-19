#include "inputs.hpp"
#include "camera_transform.hpp"
#include "event_manager.hpp"
#include "events.hpp"
#include "constants.hpp"
#include "events.hpp"
#include "inputs_events.hpp"
#include "state_manager.hpp"
#include "states.hpp"

#include "inputs_constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace inputs {
  static State inputState{};

  bool areKeysPressed(std::vector<std::vector<unsigned int>> &keys) {
    bool isCombination = true;
    for (std::vector<unsigned int> &currentCombination : keys) {
      bool keysPressed = false;
      for (unsigned int &key : currentCombination) {
        keysPressed |= (inputState.keyboardState[key] != InputAction::RELEASE);
      }
      isCombination &= keysPressed;
    }
    return isCombination;
  }

  void keyPressHandler(std::unique_ptr<core::BaseEvent> &event) {
    KeyEvent *keyEvent = static_cast<KeyEvent *>(event.get());
    if (keyEvent->type != InputAction::RELEASE)
      StateManager::getInstance().addKey(keyEvent->key);
    else
      StateManager::getInstance().removeKey(keyEvent->key);
    inputState.keyboardState[keyEvent->key] = keyEvent->type;
  }

  void mouseMovementHandler(std::unique_ptr<core::BaseEvent> &event) {
    MouseMovementEvent *mouseMovementEvent =
        static_cast<MouseMovementEvent *>(event.get());
    if (inputState.resetMousePosition == MousePositionReset::INIT) {
      inputState.resetMousePosition = MousePositionReset::DONE;
    }
    inputState.mousePosition =
        glm::vec2(mouseMovementEvent->x, mouseMovementEvent->y);
  }

  void mouseScrollHandler(std::unique_ptr<core::BaseEvent> &event) {
    MouseScrollEvent *mouseScrollEvent =
        static_cast<MouseScrollEvent *>(event.get());
    inputState.scrollDelta = mouseScrollEvent->y;
  }

  void mouseButtonHandler(std::unique_ptr<core::BaseEvent> &event) {
    MouseButtonEvent *mouseButtonEvent =
        static_cast<MouseButtonEvent *>(event.get());
    if (mouseButtonEvent->type != InputAction::RELEASE)
      StateManager::getInstance().addKey(mouseButtonEvent->button);
    else
      StateManager::getInstance().removeKey(mouseButtonEvent->button);
  }
}

namespace inputs {
  Inputs::Inputs(core::Window &window, core::Registry &registry,
                 StateKeyMap &stateManager)
      : window(window), registry(registry), stateManager(stateManager) {
    core::EventManager::getInstance().subscribe(
        core::Constants::KEY_STATE_TOPIC, keyPressHandler);
    core::EventManager::getInstance().subscribe(
        core::Constants::MOUSE_MOVEMENT_TOPIC, mouseMovementHandler);
    core::EventManager::getInstance().subscribe(
        core::Constants::MOUSE_SCROLL_TOPIC, mouseScrollHandler);
    core::EventManager::getInstance().subscribe(
        core::Constants::MOUSE_BUTTON_TOPIC, mouseButtonHandler);
  }

  void Inputs::onUpdate(float ts) {
    StateManager::getInstance().handleActivations();
    this->handleFovChange(ts);
    this->handleCameraStates();
    this->toggleCursorVisibility();
    this->updateCamera(ts);
  }

  void Inputs::handleFovChange(float ts) {
    StateType type = StateType::FOV_STATE;
    std::unordered_map<unsigned int, std::vector<std::vector<unsigned int>>>
        &inputMap = this->stateManager.getInputMap(type);
    for (std::pair<const unsigned int, std::vector<std::vector<unsigned int>>>
             &p : inputMap) {
      if (areKeysPressed(p.second)) {
        inputState.fovState = static_cast<FovState>(p.first);
        std::unique_ptr<FovChangeEvent> fovChangeEvent =
            std::make_unique<FovChangeEvent>(60.0f);
        core::EventManager::getInstance().enqueue<FovChangeEvent>(
            inputs::Constants::FOV_CHANGE_TOPIC, std::move(fovChangeEvent));
        return;
      }
    }
    inputState.fovState = FovState::NORMAL;
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

        float       xOffset = (currentPosition.x - previousPosition.x);
        float       yOffset = (previousPosition.y - currentPosition.y);
        float      &scrollDelta = inputState.scrollDelta;
        CameraState cameraState = inputState.cameraState;
        previousPosition = currentPosition;

        if (cameraState == CameraState::CAMERA_PAN_MODE) {
          cameraTransform.updateRotation(
              {xOffset * core::Constants::SPEED_SCALAR,
               yOffset * core::Constants::SPEED_SCALAR});
        } else if (cameraState == CameraState::CAMERA_FLY_MODE) {
          glm::vec3 direction(0);
          float     speed = 0.0f;
          glm::vec3 front =
              glm::normalize(cameraTransform.getForwardDirection());
          glm::vec3 right = glm::normalize(cameraTransform.getRightDirection());
          glm::vec3 up = glm::normalize(glm::cross(right, front));
          if (xOffset != 0 || yOffset != 0) {
            direction = xOffset * right + yOffset * up;
            speed = core::Constants::SPEED_SCALAR * 0.1;
          } else if (scrollDelta != 0) {
            direction = -1.0f * scrollDelta * front;
            scrollDelta = 0;
            speed = core::Constants::SPEED_SCALAR * 5;
          }
          cameraTransform.updatePosition(direction * speed);
        }
      }
    }
  }
}