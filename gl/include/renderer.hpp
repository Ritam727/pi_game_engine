#pragma once

#include "event_manager.hpp"
#include "layer.hpp"
#include "lights.hpp"
#include "logger.hpp"
#include "model.hpp"
#include "resource_manager.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "gl_utils.hpp"

#include "registry.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace gl {
  struct RenderState {
    int   width{800};
    int   height{600};
    float fov{45.0f};
  };
}

namespace gl {
  enum class DrawMode { TRIANGLES, ELEMENTS };

  class Renderer : public core::Layer {
  private:
    core::ResourceManager &resourceManager;
    std::vector<glm::vec3> pointLightPositions{
        glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

    core::Registry     &registry;
    core::EventManager &eventManager;
    Light               light{registry};

    Shader shader{ENGINE_PATH "/res/shaders/multiple_lights.vert",
                  ENGINE_PATH "/res/shaders/multiple_lights.frag"};

    unsigned int frameCount{0};
    float        cameraAngle{0};

    RenderState renderState{};
    ModelLoader modelLoader{resourceManager, registry};

    void bindLights();
    void drawObjects(float ts);

  public:
    Renderer(core::Registry &registry, core::EventManager &eventManager,
             core::ResourceManager &resourceManager, int initialWidth,
             int initialHeight);

    inline void clear() {
      GL_CALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
      GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void onUpdate(float ts) override;

    void registerWindowResizeCallback();
    void registerFovChangeCallback();
    void registerModelLoadCallback();
  };
}