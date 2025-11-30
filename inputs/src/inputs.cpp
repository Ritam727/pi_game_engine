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

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace inputs {
  Inputs::Inputs(core::Window &window, core::Registry &registry,
                 core::EventManager &eventManager)
      : window(window), registry(registry), eventManager(eventManager) {
    this->stateManager.registerMode<CameraViewMode>([&]() {
      this->eventManager.enqueue<FovChangeEvent>(
          Constants::FOV_CHANGE_TOPIC,
          this->fovMap[static_cast<unsigned int>(
              this->stateManager.getMode<CameraViewMode>()->getMode())]);
    });
    this->stateManager.registerMode<CameraMoveMode>(
        [&]() {
          this->eventManager.enqueue<core::MouseVisibleEvent>(
              Constants::MOUSE_VISIBLE_TOPIC, false);
        },
        [&]() {
          this->eventManager.enqueue<core::MouseVisibleEvent>(
              Constants::MOUSE_VISIBLE_TOPIC, true);
          this->inputState.resetMousePosition = true;
        });
    this->stateManager.loadKeyMap(ENGINE_PATH
                                  "/res/inputs/keyboard_shortcuts.json");

    this->registerKeyCallback();
    this->registerMouseButtonCallback();
    this->registerMouseMoveCallback();
    this->registerMouseScrollCallback();
  }

  void Inputs::registerKeyCallback() {
    this->eventManager.subscribe(
        core::Constants::KEY_STATE_TOPIC, [&](core::IEventPtr &event) {
          core::KeyEvent keyEvent =
              (static_cast<core::Event<core::KeyEvent> *>(event.get()))->data;
          if (keyEvent.type != core::InputAction::RELEASE)
            this->stateManager.addKey(keyEvent.key);
          else
            this->stateManager.removeKey(keyEvent.key);
        });
  }

  void Inputs::registerMouseButtonCallback() {
    this->eventManager.subscribe(
        core::Constants::MOUSE_BUTTON_TOPIC, [&](core::IEventPtr &event) {
          core::MouseButtonEvent mouseButtonEvent =
              (static_cast<core::Event<core::MouseButtonEvent> *>(event.get()))
                  ->data;
          if (mouseButtonEvent.type != core::InputAction::RELEASE)
            this->stateManager.addKey(mouseButtonEvent.button);
          else
            this->stateManager.removeKey(mouseButtonEvent.button);
        });
  }

  void Inputs::registerMouseMoveCallback() {
    this->eventManager.subscribe(
        core::Constants::MOUSE_MOVEMENT_TOPIC, [&](core::IEventPtr &event) {
          core::MouseMovementEvent mouseMovementEvent =
              (static_cast<core::Event<core::MouseMovementEvent> *>(
                   event.get()))
                  ->data;
          glm::vec2 &previousPosition = this->inputState.mousePosition;
          glm::vec2  currentPosition =
              glm::vec2(mouseMovementEvent.x, mouseMovementEvent.y);
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
          case CameraMoveMode::FLY:
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
        core::Constants::MOUSE_SCROLL_TOPIC, [&](core::IEventPtr &event) {
          this->stateManager.addKey(3);
          core::MouseScrollEvent mouseScrollEvent =
              (static_cast<core::Event<core::MouseScrollEvent> *>(event.get()))
                  ->data;
          float scrollDelta = mouseScrollEvent.y;

          CameraMoveMode cameraMoveMode =
              this->stateManager.getMode<CameraMoveMode>()->getMode();

          switch (cameraMoveMode) {
          case CameraMoveMode::FLY:
            this->cameraFly(scrollDelta);
            break;
          default:
            break;
          }

          this->stateManager.removeKey(3);
        });
  }

  void Inputs::cameraMoveAround(float xOffset, float yOffset) {
    for (core::CameraTransform &cameraTransform :
         this->registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.cameraActive) {
        glm::vec3 front = glm::normalize(cameraTransform.front);
        glm::vec3 right = glm::normalize(cameraTransform.right);
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        glm::vec3 direction = xOffset * right + yOffset * up;
        float     speed = core::Constants::SPEED_SCALAR * 0.1;
        cameraTransform.position += direction * speed;
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
      glm::vec3 front = cameraTransform.front;
      glm::vec3 direction = -1.0f * delta * front;

      float speed = core::Constants::SPEED_SCALAR;
      cameraTransform.position += direction * speed;
    }
  }
}