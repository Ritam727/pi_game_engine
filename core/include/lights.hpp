#pragma once

#include "base_component.hpp"
#include "base_object.hpp"
#include "glm/glm.hpp"
#include "selectable.hpp"
#include "utils.hpp"

namespace core {
  ITERABLE_ENUM(LightType, DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT);

  struct LightComponent : public core::BaseComponent {
    LightType type{LightType::DIRECTIONAL_LIGHT};

    glm::vec3 direction{0.0f, -1.0f, -1.0f};
    glm::vec3 position{0.0f, 10.0f, 0.0f};

    glm::vec3 ambient{0.1f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};

    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};

    float innerCutOff{12.5f};
    float outerCutOff{17.5f};

    LightComponent() {}

    LightComponent(LightType type) : type(type) {}
  };
}

namespace core {
  class Light : public core::BaseObject {
  public:
    Light(core::Registry &registryRef) : core::BaseObject(registryRef) {
      registryRef.addComponent<core::Selectable>(this->getEntityId());
    }
  };
}