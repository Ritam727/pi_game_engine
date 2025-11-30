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