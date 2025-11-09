#include "camera_transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace gl {
  CameraTransform::CameraTransform(ecs::Entity    entityId,
                                   ecs::Registry &registryRef,
                                   glm::vec3 position, glm::vec3 up)
      : BaseComponent(registryRef), position(position), target(0), up(up),
        right(glm::normalize(
            glm::cross(glm::normalize(this->position - this->target),
                       glm::normalize(this->up)))) {}

  void CameraTransform::setPosition(glm::vec3 position) {
    this->position = position;
  }

  glm::mat4 CameraTransform::getViewMatrix() const {
    return glm::lookAt(this->position, this->target, this->up);
  }
}