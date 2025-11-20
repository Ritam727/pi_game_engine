#include "inputs.hpp"
#include "camera_transform.hpp"
#include "event_manager.hpp"
#include "constants.hpp"
#include "inputs_events.hpp"
#include "modes.hpp"
#include "state_manager.hpp"
#include "states.hpp"

#include "inputs_constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace inputs {
  Inputs::Inputs(core::Window &window, core::Registry &registry,
                 core::EventManager &eventManager)
      : window(window), registry(registry), eventManager(eventManager) {
    stateManager.registerMode<CameraViewMode>();
    stateManager.registerMode<CameraMoveMode>();

    this->eventManager.subscribe(
        core::Constants::KEY_STATE_TOPIC, [&](core::BaseEventPtr &event) {
          KeyEvent *keyEvent = static_cast<KeyEvent *>(event.get());
          if (keyEvent->type != InputAction::RELEASE)
            this->stateManager.addKey(keyEvent->key);
          else
            this->stateManager.removeKey(keyEvent->key);
          this->inputState.keyboardState[keyEvent->key] = keyEvent->type;
        });
    this->eventManager.subscribe(
        core::Constants::MOUSE_MOVEMENT_TOPIC, [&](core::BaseEventPtr &event) {
          MouseMovementEvent *mouseMovementEvent =
              static_cast<MouseMovementEvent *>(event.get());
          if (this->inputState.resetMousePosition == MousePositionReset::INIT) {
            this->inputState.resetMousePosition = MousePositionReset::DONE;
          }
          this->inputState.mousePosition =
              glm::vec2(mouseMovementEvent->x, mouseMovementEvent->y);
        });
    this->eventManager.subscribe(
        core::Constants::MOUSE_SCROLL_TOPIC, [&](core::BaseEventPtr &event) {
          MouseScrollEvent *mouseScrollEvent =
              static_cast<MouseScrollEvent *>(event.get());
          this->inputState.scrollDelta = mouseScrollEvent->y;
        });
    this->eventManager.subscribe(
        core::Constants::MOUSE_BUTTON_TOPIC, [&](core::BaseEventPtr &event) {
          MouseButtonEvent *mouseButtonEvent =
              static_cast<MouseButtonEvent *>(event.get());
          if (mouseButtonEvent->type != InputAction::RELEASE)
            this->stateManager.addKey(mouseButtonEvent->button);
          else
            this->stateManager.removeKey(mouseButtonEvent->button);
        });
  }

  void Inputs::onUpdate(float ts) {
    this->handleFovChange(ts);
    this->toggleCursorVisibility();
    this->updateCamera(ts);
  }

  void Inputs::handleFovChange(float ts) {
    CameraViewMode cameraViewMode =
        stateManager.getMode<CameraViewMode>()->getMode();
    if (cameraViewMode != this->inputState.cameraViewMode) {
      float fov = 45.0f;
      if (cameraViewMode == CameraViewMode::BIRD_EYE) {
        fov = 60.0f;
      } else if (cameraViewMode == CameraViewMode::FISH_EYE) {
        fov = 75.0f;
      }
      std::unique_ptr<FovChangeEvent> fovChangeEvent =
          std::make_unique<FovChangeEvent>(fov);
      this->eventManager.enqueue<FovChangeEvent>(
          inputs::Constants::FOV_CHANGE_TOPIC, std::move(fovChangeEvent));
      this->inputState.cameraViewMode = cameraViewMode;
    }
  }

  void Inputs::toggleCursorVisibility() {
    CameraMoveMode cameraMoveMode =
        stateManager.getMode<CameraMoveMode>()->getMode();
    if (cameraMoveMode == CameraMoveMode::PAN ||
        cameraMoveMode == CameraMoveMode::FLY ||
        cameraMoveMode == CameraMoveMode::MOVE_AROUND) {
      this->window.setCursorVisibility(false);
    } else {
      this->window.setCursorVisibility(true);
      this->inputState.resetMousePosition = MousePositionReset::INIT;
    }
  }

  void Inputs::updateCamera(float ts) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        if (this->inputState.cameraState == CameraState::CAMERA_RESET_MODE) {
          cameraTransform.resetCameraTransform();
          break;
        }
        if (this->inputState.resetMousePosition == MousePositionReset::DONE) {
          this->mousePosition = this->inputState.mousePosition;
          this->inputState.resetMousePosition = MousePositionReset::NONE;
        }
        glm::vec2 &currentPosition = this->inputState.mousePosition;
        glm::vec2 &previousPosition = this->mousePosition;

        float          xOffset = (currentPosition.x - previousPosition.x);
        float          yOffset = (previousPosition.y - currentPosition.y);
        float         &scrollDelta = this->inputState.scrollDelta;
        CameraMoveMode cameraMoveMode =
            stateManager.getMode<CameraMoveMode>()->getMode();
        previousPosition = currentPosition;

        if (cameraMoveMode == CameraMoveMode::PAN) {
          cameraTransform.updateRotation(
              {xOffset * core::Constants::SPEED_SCALAR,
               yOffset * core::Constants::SPEED_SCALAR});
        }

        glm::vec3 direction(0);
        float     speed = 0.0f;
        glm::vec3 front = glm::normalize(cameraTransform.getForwardDirection());
        glm::vec3 right = glm::normalize(cameraTransform.getRightDirection());
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        if (cameraMoveMode == CameraMoveMode::MOVE_AROUND) {
          if (xOffset != 0 || yOffset != 0) {
            direction = xOffset * right + yOffset * up;
            speed = core::Constants::SPEED_SCALAR * 0.1;
          }
        } else if (cameraMoveMode == CameraMoveMode::FLY) {
          if (scrollDelta != 0) {
            direction = -1.0f * scrollDelta * front;
            scrollDelta = 0;
            speed = core::Constants::SPEED_SCALAR * ts;
          }
        }
        cameraTransform.updatePosition(direction * speed);
      }
    }
  }
}