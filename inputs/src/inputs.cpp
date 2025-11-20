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
    this->stateManager.registerMode<CameraViewMode>();
    this->stateManager.registerMode<CameraMoveMode>();

    this->registerKeyCallback();
    this->registerMouseButtonCallback();
    this->registerMouseMoveCallback();
    this->registerMouseScrollCallback();
  }

  void Inputs::registerKeyCallback() {
    this->eventManager.subscribe(
        core::Constants::KEY_STATE_TOPIC, [&](core::BaseEventPtr &event) {
          core::KeyEvent *keyEvent = static_cast<core::KeyEvent *>(event.get());
          if (keyEvent->type != core::InputAction::RELEASE)
            this->stateManager.addKey(keyEvent->key);
          else
            this->stateManager.removeKey(keyEvent->key);
        });
  }

  void Inputs::registerMouseButtonCallback() {
    this->eventManager.subscribe(
        core::Constants::MOUSE_BUTTON_TOPIC, [&](core::BaseEventPtr &event) {
          core::MouseButtonEvent *mouseButtonEvent =
              static_cast<core::MouseButtonEvent *>(event.get());
          if (mouseButtonEvent->type != core::InputAction::RELEASE)
            this->stateManager.addKey(mouseButtonEvent->button);
          else
            this->stateManager.removeKey(mouseButtonEvent->button);
        });
  }

  void Inputs::registerMouseMoveCallback() {
    this->eventManager.subscribe(
        core::Constants::MOUSE_MOVEMENT_TOPIC, [&](core::BaseEventPtr &event) {
          core::MouseMovementEvent *mouseMovementEvent =
              static_cast<core::MouseMovementEvent *>(event.get());
          glm::vec2 &previousPosition = this->inputState.mousePosition;
          glm::vec2  currentPosition =
              glm::vec2(mouseMovementEvent->x, mouseMovementEvent->y);
          CameraMoveMode cameraMoveMode =
              this->stateManager.getMode<CameraMoveMode>()->getMode();

          if (this->inputState.resetMousePosition) {
            previousPosition = currentPosition;
            this->inputState.resetMousePosition = false;
            return;
          }

          float xOffset = (currentPosition.x - previousPosition.x);
          float yOffset = (previousPosition.y - currentPosition.y);

          switch (cameraMoveMode) {
          case CameraMoveMode::MOVE_AROUND:
            this->cameraMoveAround(xOffset, yOffset);
            break;
          case CameraMoveMode::PAN:
            this->cameraPan(xOffset, yOffset);
            break;
          default:
            break;
          }

          previousPosition = currentPosition;
        });
  }

  void Inputs::registerMouseScrollCallback() {
    this->eventManager.subscribe(
        core::Constants::MOUSE_SCROLL_TOPIC, [&](core::BaseEventPtr &event) {
          core::MouseScrollEvent *mouseScrollEvent =
              static_cast<core::MouseScrollEvent *>(event.get());
          float scrollDelta = mouseScrollEvent->y;

          CameraMoveMode cameraMoveMode =
              this->stateManager.getMode<CameraMoveMode>()->getMode();

          switch (cameraMoveMode) {
          case CameraMoveMode::FLY:
            this->cameraFly(scrollDelta);
            break;
          default:
            return;
          }
        });
  }

  void Inputs::cameraMoveAround(float xOffset, float yOffset) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        glm::vec3 front = glm::normalize(cameraTransform.getForwardDirection());
        glm::vec3 right = glm::normalize(cameraTransform.getRightDirection());
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        glm::vec3 direction = xOffset * right + yOffset * up;
        float     speed = core::Constants::SPEED_SCALAR * 0.1;
        cameraTransform.updatePosition(direction * speed);
      }
    }
  }

  void Inputs::cameraPan(float xOffset, float yOffset) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      cameraTransform.updateRotation({xOffset * core::Constants::SPEED_SCALAR,
                                      yOffset * core::Constants::SPEED_SCALAR});
    }
  }

  void Inputs::cameraFly(float delta) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      glm::vec3 front = cameraTransform.getForwardDirection();
      glm::vec3 direction = -1.0f * delta * front;

      float speed = core::Constants::SPEED_SCALAR;
      cameraTransform.updatePosition(direction * speed);
    }
  }

  void Inputs::onUpdate(float ts) {
    this->handleFovChange();
    this->toggleCursorVisibility();
  }

  void Inputs::handleFovChange() {
    CameraViewMode cameraViewMode =
        this->stateManager.getMode<CameraViewMode>()->getMode();
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
        this->stateManager.getMode<CameraMoveMode>()->getMode();
    switch (cameraMoveMode) {
    case CameraMoveMode::PAN:
    case CameraMoveMode::FLY:
    case CameraMoveMode::MOVE_AROUND:
      this->window.setCursorVisibility(false);
      break;
    default:
      this->window.setCursorVisibility(true);
      this->inputState.resetMousePosition = true;
    }
  }
}