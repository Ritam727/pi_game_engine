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
    Texture diffuse;
    Texture specular;

    float shininess;

    TextureMaterial(std::string diffusePath, std::string specularPath,
                    float shininess)
        : diffuse(diffusePath), specular(specularPath), shininess(shininess) {}

    void clearComponent() override {
      diffuse.releaseTexture();
      specular.releaseTexture();
    }
  };
}