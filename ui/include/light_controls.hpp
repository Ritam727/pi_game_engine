#pragma once

#include "registry.hpp"
#include "lights.hpp"

namespace ui {
  class LightControls {
  private:
    static void directionalLightControls(const core::Entity &entity,
                                         gl::LightComponent &lightComponent);
    static void pointLightControls(const core::Entity &entity,
                                   gl::LightComponent &lightComponent);
    static void spotLightControls(const core::Entity &entity,
                                  gl::LightComponent &lightComponent);

  public:
    static void lightComponent(const core::Entity &entity,
                               gl::LightComponent &lightComponent);
  };
}