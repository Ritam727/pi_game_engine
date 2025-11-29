#pragma once

#include "glm/glm.hpp"

namespace ui {
  class TransformControls {
  private:
    static void translationControls(glm::vec3 &translation);
    static void rotationControls(glm::vec3 &rotation);
    static void scaleControls(glm::vec3 &rotation);

  public:
    static void transformControls(glm::vec3 &translation, glm::vec3 &rotation,
                                  glm::vec3 &scale);
  };
}