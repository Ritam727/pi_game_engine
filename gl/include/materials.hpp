#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"
#include "resource_manager.hpp"
#include "texture.hpp"
#include <optional>
#include <thread>
#include <variant>

namespace gl {
  struct MaterialAttribute {
    enum class MaterialAttributeType { COLOR, TEXTURE } type;

    std::variant<glm::vec3, std::string> attribute;

    MaterialAttribute(glm::vec3 vec)
        : type(MaterialAttributeType::COLOR), attribute(vec) {}

    MaterialAttribute(std::string filePath)
        : type(MaterialAttributeType::TEXTURE), attribute(filePath) {}
  };
}

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