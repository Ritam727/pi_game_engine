#include "camera.hpp"

namespace core {
  Camera::Camera(core::Registry &registryRef, glm::vec3 position, glm::vec3 up)
      : BaseObject(registryRef), transform(position, up) {
    registryRef.addComponent<CameraTransform>(this->getEntityId(), position,
                                              up);
  }

  CameraTransform &Camera::getCameraTransform() {
    return this->transform;
  }
}