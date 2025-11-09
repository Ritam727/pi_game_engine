#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"

namespace gl {
  class CameraTransform : public BaseComponent {
  private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 right;

  public:
    CameraTransform(ecs::Entity entityId, ecs::Registry &registryRef,
                    glm::vec3 position, glm::vec3 up);

    void setPosition(glm::vec3 position);

    glm::mat4 getViewMatrix() const;
  };
}