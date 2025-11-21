#include "camera_transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace core {
  CameraTransform::CameraTransform(glm::vec3 position, glm::vec3 up)
      : position(position),
        front(glm::normalize(
            glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                      sin(glm::radians(pitch)),
                      sin(glm::radians(yaw)) * cos(glm::radians(pitch))))),
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
    this->yaw += delta.x;
    this->pitch += delta.y;
    if (this->pitch > 89.0f)
      this->pitch = 89.0f;
    if (this->pitch < -89.0f)
      this->pitch = -89.0f;

    this->front = glm::normalize(
        glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                  sin(glm::radians(pitch)),
                  sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
    this->right = glm::normalize(glm::cross(this->front, this->up));
  }

  void CameraTransform::resetCameraTransform() {
    this->yaw = -90.0f;
    this->pitch = 0.0f;
    this->position = glm::vec3(0.0f, 0.0f, 3.0f);
    this->front = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                            sin(glm::radians(pitch)),
                            sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
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

  glm::mat4 CameraTransform::getViewMatrix() const {
    return glm::lookAt(this->position, this->position + this->front, this->up);
  }
}