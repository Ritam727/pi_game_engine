#pragma once

#include "registry.hpp"
#include "lights.hpp"

namespace ui {
  class LightControls {
  private:
    static void directionalLightControls(core::Entity       &entity,
                                         gl::LightComponent &lightComponent);
    static void pointLightControls(core::Entity       &entity,
                                   gl::LightComponent &lightComponent);
    static void spotLightControls(core::Entity       &entity,
                                  gl::LightComponent &lightComponent);

  public:
    static void lightComponent(core::Entity       &entity,
                               gl::LightComponent &lightComponent);
  };
}