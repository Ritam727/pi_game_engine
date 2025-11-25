#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"

namespace core {
  class CameraTransform : public BaseComponent {
  private:
    float yaw{-90.0f};
    float pitch{0.0f};

    glm::vec3 position{0.0f};
    glm::vec3 front{0.0f};
    glm::vec3 up{0.0f};
    glm::vec3 right{0.0f};

    bool cameraActive{false};

  public:
    CameraTransform();
    CameraTransform(glm::vec3 position, glm::vec3 up);

    void setPosition(glm::vec3 position);

    void updatePosition(glm::vec3 delta);
    void setCameraActive(bool cameraActive);
    void updateRotation(glm::vec2 delta);
    void resetCameraTransform();

    bool isCameraActive() const;

    glm::vec3 getForwardDirection() const;
    glm::vec3 getRightDirection() const;
    glm::vec3 getPosition() const;

    glm::mat4 getViewMatrix() const;
  };
}