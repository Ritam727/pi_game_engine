#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"
#include "texture.hpp"
#include <optional>

namespace gl {
  struct MaterialGroup {
    glm::vec3              vector;
    std::optional<Texture> texture;
  };

  struct Material : public core::BaseComponent {
    MaterialGroup ambient;
    MaterialGroup diffuse;
    MaterialGroup specular;

    float shininess;

    Material(MaterialGroup ambient, MaterialGroup diffuse,
             MaterialGroup specular, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          shininess(shininess) {}
  };
}