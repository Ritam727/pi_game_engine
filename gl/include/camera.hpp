#pragma once

#include "base_object.hpp"
#include "camera_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace core {
  class Camera : public BaseObject {
  private:
    CameraTransform transform;

  public:
    Camera(ecs::Registry &registryRef, glm::vec3 position, glm::vec3 up);

    CameraTransform &getCameraTransform();
  };
}