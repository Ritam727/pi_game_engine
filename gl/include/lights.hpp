#pragma once

#include "base_component.hpp"
#include "base_object.hpp"
#include "glm/glm.hpp"
#include "selectable.hpp"

namespace gl {
  struct PointLight : public core::BaseComponent {
    glm::vec3 position{0.0f, 3.0f, 3.0f};

    glm::vec3 ambient{0.0f};
    glm::vec3 diffuse{0.0f};
    glm::vec3 specular{0.0f};

    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};

    PointLight() {}

    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : ambient(ambient), diffuse(diffuse), specular(specular) {}
  };

  struct PointLights : public core::BaseComponent {
    std::vector<PointLight> lights{};

    PointLights() {}

    PointLights(std::vector<PointLight> lights) : lights(lights) {}
  };

  struct DirectionalLight : public core::BaseComponent {
    glm::vec3 direction{0.0f, -1.0f, -1.0f};

    glm::vec3 ambient{0.0f};
    glm::vec3 diffuse{0.0f};
    glm::vec3 specular{0.0f};

    DirectionalLight() {}

    DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : ambient(ambient), diffuse(diffuse), specular(specular) {}
  };

  struct SpotLight : public PointLight {
    glm::vec3 direction{0.0f, -1.0f, -1.0f};
    float     innerCutOff{1.0f};
    float     outerCutOff{1.0f};

    SpotLight() {}

    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
              float innerCutOff, float outerCutOff)
        : PointLight(ambient, diffuse, specular), innerCutOff(innerCutOff),
          outerCutOff(outerCutOff) {}
  };

  class Light : public core::BaseObject {
  public:
    Light(core::Registry &registryRef) : core::BaseObject(registryRef) {
      registryRef.addComponent<core::Selectable>(this->getEntityId());
    }
  };
}