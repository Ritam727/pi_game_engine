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
    std::vector<Material> materials{{{0.0215, 0.1745, 0.0215},
                                     {0.07568, 0.61424, 0.07568},
                                     {0.633, 0.727811, 0.633},
                                     0.6 * 128},
                                    {{0.135, 0.2225, 0.1575},
                                     {0.54, 0.89, 0.63},
                                     {0.316228, 0.316228, 0.316228},
                                     0.1 * 128},
                                    {{0.05375, 0.05, 0.06625},
                                     {0.18275, 0.17, 0.22525},
                                     {0.332741, 0.328634, 0.346435},
                                     0.3 * 128},
                                    {{0.25, 0.20725, 0.20725},
                                     {1, 0.829, 0.829},
                                     {0.296648, 0.296648, 0.296648},
                                     0.088 * 128},
                                    {{0.1745, 0.01175, 0.01175},
                                     {0.61424, 0.04136, 0.04136},
                                     {0.727811, 0.626959, 0.626959},
                                     0.6 * 128},
                                    {{0.1, 0.18725, 0.1745},
                                     {0.396, 0.74151, 0.69102},
                                     {0.297254, 0.30829, 0.306678},
                                     0.1 * 128},
                                    {{0.329412, 0.223529, 0.027451},
                                     {0.780392, 0.568627, 0.113725},
                                     {0.992157, 0.941176, 0.807843},
                                     0.21794872 * 128},
                                    {{0.2125, 0.1275, 0.054},
                                     {0.714, 0.4284, 0.18144},
                                     {0.393548, 0.271906, 0.166721},
                                     0.2 * 128},
                                    {{0.25, 0.25, 0.25},
                                     {0.4, 0.4, 0.4},
                                     {0.774597, 0.774597, 0.774597},
                                     0.6 * 128},
                                    {{0.19125, 0.0735, 0.0225},
                                     {0.7038, 0.27048, 0.0828},
                                     {0.256777, 0.137622, 0.086014},
                                     0.1 * 128}};

    core::Registry     &registry;
    core::EventManager &eventManager;

    VertexArray vertexArray{vertices, indices};
    Shader      shader{ENGINE_PATH "/res/shaders/basic.vert",
                  ENGINE_PATH "/res/shaders/basic.frag"};
    Texture     texture{{ENGINE_PATH "/res/textures/container.jpg",
                         ENGINE_PATH "/res/textures/awesomeface.png"}};

    unsigned int frameCount{0};
    float        cameraAngle{0};
    DrawMode     drawMode{DrawMode::TRIANGLES};

    RenderState renderState{};

    BasicLight basicLight{{0.2f, 0.2f, 0.2f},
                          {0.5f, 0.5f, 0.5f},
                          {1.0f, 1.0f, 1.0f}};

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