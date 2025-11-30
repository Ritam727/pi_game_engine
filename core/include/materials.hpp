#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"

namespace core {
  struct MaterialGroup {
    glm::vec3 vector{0.0f};
    int       texture{-1};
  };

  struct Material : public core::BaseComponent {
    MaterialGroup ambient{};
    MaterialGroup diffuse{};
    MaterialGroup specular{};

    float shininess{32.0f};

    Material() {}

    Material(MaterialGroup ambient, MaterialGroup diffuse,
             MaterialGroup specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          shininess(shininess) {}
  };
}