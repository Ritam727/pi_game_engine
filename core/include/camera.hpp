#pragma once

#include "base_object.hpp"
#include "camera_transform.hpp"
#include "glm/glm.hpp"

namespace core {
  class Camera : public BaseObject {
  private:
    CameraTransform transform;

  public:
    Camera(core::Registry &registryRef, glm::vec3 position, glm::vec3 up);

    CameraTransform &getCameraTransform();
  };
}