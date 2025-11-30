#include "camera_transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace core {
  CameraTransform::CameraTransform() {}

  CameraTransform::CameraTransform(glm::vec3 position, glm::vec3 up)
      : position(position),
        front(glm::normalize(glm::vec3(
            cos(glm::radians(angles.x)) * cos(glm::radians(angles.y)),
            sin(glm::radians(angles.y)),
            sin(glm::radians(angles.x)) * cos(glm::radians(angles.y))))),
        up(up), right(glm::normalize(glm::cross(glm::normalize(this->front),
                                                glm::normalize(this->up)))) {}

  void CameraTransform::setPosition(glm::vec3 position) {
    this->position = position;
  }

  void CameraTransform::updatePosition(glm::vec3 delta) {
    this->position += delta;
  }

  void CameraTransform::setCameraActive(bool cameraActive) {
    this->cameraActive = cameraActive;
  }

  void CameraTransform::updateRotation(glm::vec2 delta) {
    this->angles.x += delta.x;
    this->angles.y += delta.y;
    if (this->angles.y > 89.0f)
      this->angles.y = 89.0f;
    if (this->angles.y < -89.0f)
      this->angles.y = -89.0f;

    this->front = glm::normalize(
        glm::vec3(cos(glm::radians(angles.x)) * cos(glm::radians(angles.y)),
                  sin(glm::radians(angles.y)),
                  sin(glm::radians(angles.x)) * cos(glm::radians(angles.y))));
    this->right = glm::normalize(glm::cross(this->front, this->up));
  }

  void CameraTransform::resetCameraTransform() {
    this->angles.x = -90.0f;
    this->angles.y = 0.0f;
    this->position = glm::vec3(0.0f, 0.0f, 3.0f);
    this->front =
        glm::vec3(cos(glm::radians(angles.x)) * cos(glm::radians(angles.y)),
                  sin(glm::radians(angles.y)),
                  sin(glm::radians(angles.x)) * cos(glm::radians(angles.y)));
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->right = glm::normalize(
        glm::cross(glm::normalize(this->front), glm::normalize(this->up)));
  }

  bool CameraTransform::isCameraActive() const {
    return this->cameraActive;
  }

  glm::vec3 CameraTransform::getForwardDirection() const {
    return this->front;
  }

  glm::vec3 CameraTransform::getRightDirection() const {
    return this->right;
  }

  glm::vec3 CameraTransform::getPosition() const {
    return this->position;
  }

  glm::mat4 CameraTransform::getViewMatrix() {
    this->front =
        glm::vec3(cos(glm::radians(angles.x)) * cos(glm::radians(angles.y)),
                  sin(glm::radians(angles.y)),
                  sin(glm::radians(angles.x)) * cos(glm::radians(angles.y)));
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->right = glm::normalize(
        glm::cross(glm::normalize(this->front), glm::normalize(this->up)));
    return glm::lookAt(this->position, this->position + this->front, this->up);
  }
}