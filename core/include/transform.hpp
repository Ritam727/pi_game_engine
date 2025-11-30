#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace core {
  struct Transform : public BaseComponent {
    glm::vec3 position{0};
    glm::vec3 scale{1};
    glm::quat rotation{glm::radians(glm::vec3(0.0f))};

    glm::mat4 getTranslationMatrix() const;
    glm::mat4 getRotationMatrix() const;
    glm::mat4 getScaleMatrix() const;

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

    void updatePosition(glm::vec3 delta);
    void updateRotation(glm::vec3 delta);
    void updateScale(glm::vec3 delta);

    glm::mat4 getModelMatrix() const;
    glm::quat getRotation() const;
  };
}