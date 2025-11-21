#pragma once

#include "glm/glm.hpp"

namespace gl {
  struct BasicLight {
    glm::vec3 position{3.0f, 0.0f, 3.0f};
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    BasicLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : ambient(ambient), diffuse(diffuse), specular(specular) {}
  };
}