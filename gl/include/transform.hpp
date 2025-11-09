#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "sparse_set.hpp"

namespace gl {
  class Transform : public BaseComponent {
  private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 axisX;
    glm::vec3 axisY;
    glm::vec3 axisZ;
    glm::quat rotation;

    glm::mat4 getTranslationMatrix() const;
    glm::mat4 getRotationMatrix() const;
    glm::mat4 getScaleMatrix() const;

  public:
    Transform(ecs::Entity entityId, ecs::Registry &registryRef);

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

    void updateRotation(glm::vec3 delta);

    glm::mat4 getModelMatrix() const;
    glm::quat getRotation() const;
  };
}