#pragma once

#include "glm/glm.hpp"

namespace gl {
  struct PointLight {
    glm::vec3 position{0.0f, 3.0f, 3.0f};

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};

    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : ambient(ambient), diffuse(diffuse), specular(specular) {}
  };

  struct DirectionalLight {
    glm::vec3 direction{0.0f, -1.0f, -1.0f};

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : ambient(ambient), diffuse(diffuse), specular(specular) {}
  };

  struct SpotLight : public PointLight {
    glm::vec3 direction{0.0f, -1.0f, -1.0f};
    float     innerCutOff;
    float     outerCutOff;

    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
              float innerCutOff, float outerCutOff)
        : PointLight(ambient, diffuse, specular), innerCutOff(innerCutOff),
          outerCutOff(outerCutOff) {}
  };
}