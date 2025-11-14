#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"

namespace core {
  class CameraTransform : public BaseComponent {
  private:
    float yaw;
    float pitch;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    bool cameraActive;

  public:
    CameraTransform(core::Entity entityId, core::Registry &registryRef,
                    glm::vec3 position, glm::vec3 up);

    void setPosition(glm::vec3 position);

    void updatePosition(glm::vec3 delta);
    void setCameraActive(bool cameraActive);
    void updateRotation(glm::vec2 delta);
    void resetCameraTransform();

    bool isCameraActive() const;

    glm::vec3 getForwardDirection() const;
    glm::vec3 getRightDirection() const;

    glm::mat4 getViewMatrix() const;
  };
}