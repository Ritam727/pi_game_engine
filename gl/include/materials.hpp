#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"

namespace gl {
  struct Material : public core::BaseComponent {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
             float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          shininess(shininess) {}
  };
}