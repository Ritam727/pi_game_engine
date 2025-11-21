#pragma once

#include "event_manager.hpp"
#include "layer.hpp"
#include "lights.hpp"
#include "logger.hpp"
#include "materials.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "gl_utils.hpp"
#include "vertex_array.hpp"

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
    std::vector<core::Vertex> vertices{{{-0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, -1.0f},
                                        {0.0f, 0.0f}},
                                       {{0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, -1.0f},
                                        {1.0f, 0.0f}},
                                       {{0.5f, 0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, -1.0f},
                                        {1.0f, 1.0f}},
                                       {{0.5f, 0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, -1.0f},
                                        {1.0f, 1.0f}},
                                       {{-0.5f, 0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, -1.0f},
                                        {0.0f, 1.0f}},
                                       {{-0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, -1.0f},
                                        {0.0f, 0.0f}},
                                       {{-0.5f, -0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, 1.0f},
                                        {0.0f, 0.0f}},
                                       {{0.5f, -0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, 1.0f},
                                        {1.0f, 0.0f}},
                                       {{0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, 1.0f},
                                        {1.0f, 1.0f}},
                                       {{0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, 1.0f},
                                        {1.0f, 1.0f}},
                                       {{-0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, 1.0f},
                                        {0.0f, 1.0f}},
                                       {{-0.5f, -0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 0.0f, 1.0f},
                                        {0.0f, 0.0f}},
                                       {{-0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {-1.0f, 0.0f, 0.0f},
                                        {1.0f, 0.0f}},
                                       {{-0.5f, 0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {-1.0f, 0.0f, 0.0f},
                                        {1.0f, 1.0f}},
                                       {{-0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {-1.0f, 0.0f, 0.0f},
                                        {0.0f, 1.0f}},
                                       {{-0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {-1.0f, 0.0f, 0.0f},
                                        {0.0f, 1.0f}},
                                       {{-0.5f, -0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {-1.0f, 0.0f, 0.0f},
                                        {0.0f, 0.0f}},
                                       {{-0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {-1.0f, 0.0f, 0.0f},
                                        {1.0f, 0.0f}},
                                       {{0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {1.0f, 0.0f, 0.0f},
                                        {1.0f, 0.0f}},
                                       {{0.5f, 0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {1.0f, 0.0f, 0.0f},
                                        {1.0f, 1.0f}},
                                       {{0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {1.0f, 0.0f, 0.0f},
                                        {0.0f, 1.0f}},
                                       {{0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {1.0f, 0.0f, 0.0f},
                                        {0.0f, 1.0f}},
                                       {{0.5f, -0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {1.0f, 0.0f, 0.0f},
                                        {0.0f, 0.0f}},
                                       {{0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {1.0f, 0.0f, 0.0f},
                                        {1.0f, 0.0f}},
                                       {{-0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, -1.0f, 0.0f},
                                        {0.0f, 1.0f}},
                                       {{0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, -1.0f, 0.0f},
                                        {1.0f, 1.0f}},
                                       {{0.5f, -0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, -1.0f, 0.0f},
                                        {1.0f, 0.0f}},
                                       {{0.5f, -0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, -1.0f, 0.0f},
                                        {1.0f, 0.0f}},
                                       {{-0.5f, -0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, -1.0f, 0.0f},
                                        {0.0f, 0.0f}},
                                       {{-0.5f, -0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, -1.0f, 0.0f},
                                        {0.0f, 1.0f}},
                                       {{-0.5f, 0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 1.0f, 0.0f},
                                        {0.0f, 1.0f}},
                                       {{0.5f, 0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 1.0f, 0.0f},
                                        {1.0f, 1.0f}},
                                       {{0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 1.0f, 0.0f},
                                        {1.0f, 0.0f}},
                                       {{0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 1.0f, 0.0f},
                                        {1.0f, 0.0f}},
                                       {{-0.5f, 0.5f, 0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 1.0f, 0.0f},
                                        {0.0f, 0.0f}},
                                       {{-0.5f, 0.5f, -0.5f},
                                        {0.8f, 0.1f, 0.2f},
                                        {0.0f, 1.0f, 0.0f},
                                        {0.0f, 1.0f}}};
    std::vector<unsigned int> indices{};
    std::vector<glm::vec3>    cubePositions{
           {0.0f, 0.0f, 0.0f},     {2.0f, 5.0f, -15.0f}, {-1.5f, -2.2f, -2.5f},
           {-3.8f, -2.0f, -12.3f}, {2.4f, -0.4f, -3.5f}, {-1.7f, 3.0f, -7.5f},
           {1.3f, -2.0f, -2.5f},   {1.5f, 2.0f, -2.5f},  {1.5f, 0.2f, -1.5f},
           {-1.3f, 1.0f, -1.5f}};

    BasicLight          basicLight{{0.2f, 0.2f, 0.2f},
                                   {0.5f, 0.5f, 0.5f},
                                   {1.0f, 1.0f, 1.0f}};
    core::Registry     &registry;
    core::EventManager &eventManager;

    VertexArray vertexArray{vertices, indices};
    Shader      shader{ENGINE_PATH "/res/shaders/texture_material.vert",
                  ENGINE_PATH "/res/shaders/texture_material.frag"};

    unsigned int frameCount{0};
    float        cameraAngle{0};
    DrawMode     drawMode{DrawMode::TRIANGLES};

    RenderState renderState{};

    void draw();

  public:
    Renderer(core::Registry &registry, core::EventManager &eventManager);

    inline void clear() {
      GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
      GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void onUpdate(float ts) override;
  };

}