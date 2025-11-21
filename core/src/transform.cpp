#include "transform.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace core {
  Transform::Transform() {}

  glm::mat4 Transform::getTranslationMatrix() const {
    return glm::translate(glm::mat4(1.0f), this->position);
  }

  glm::mat4 Transform::getRotationMatrix() const {
    return glm::mat4_cast(glm::normalize(this->rotation));
  }

  glm::mat4 Transform::getScaleMatrix() const {
    return glm::scale(glm::mat4(1.0f), this->scale);
  }

  void Transform::setPosition(glm::vec3 position) {
    this->position = position;
  }

  void Transform::setRotation(glm::vec3 rotation) {
    this->rotation = glm::normalize(glm::quat(glm::radians(rotation)));
  }

  void Transform::setScale(glm::vec3 scale) {
    this->scale = scale;
  }

  void Transform::updateRotation(glm::vec3 delta) {
    glm::quat q = glm::normalize(glm::quat(glm::radians(delta)));
    this->rotation = glm::normalize(q * this->rotation);
  }

  glm::mat4 Transform::getModelMatrix() const {
    return this->getTranslationMatrix() * this->getRotationMatrix() *
           this->getScaleMatrix();
  }

  glm::quat Transform::getRotation() const {
    return this->rotation;
  }
}