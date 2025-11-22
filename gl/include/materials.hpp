#pragma once

#include "base_component.hpp"
#include "glm/glm.hpp"
#include "texture.hpp"

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

  struct TextureMaterial : public core::BaseComponent {
    Texture textures;

    float shininess;

    TextureMaterial(std::vector<std::string> textures, glm::vec3 specular,
                    float shininess)
        : textures(textures), shininess(shininess) {}

    void clearComponent() override {
      textures.releaseTextures();
    }
  };
}