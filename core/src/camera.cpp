#include "camera.hpp"
#include "selectable.hpp"

namespace core {
  Camera::Camera(core::Registry &registryRef, glm::vec3 position, glm::vec3 up)
      : BaseObject(registryRef), transform(position, up) {
    registryRef.addComponent<CameraTransform>(this->getEntityId(), position,
                                              up);
    registryRef.addComponent<Selectable>(this->getEntityId());
  }

  CameraTransform &Camera::getCameraTransform() {
    return this->transform;
  }
}