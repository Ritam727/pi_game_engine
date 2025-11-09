#include "camera.hpp"

namespace gl {
  Camera::Camera(ecs::Registry &registryRef, glm::vec3 position, glm::vec3 up)
      : BaseObject(registryRef),
        transform(this->getEntityId(), registryRef, position, up) {}

  CameraTransform &Camera::getCameraTransform() {
    return this->transform;
  }
}