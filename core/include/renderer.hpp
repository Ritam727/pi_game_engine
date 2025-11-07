#pragma once

#include "logger.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "events.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "transform.hpp"

#include "registry.hpp"

#include "event_manager.hpp"

#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"

namespace core {
  enum class DrawMode { TRIANGLES, ELEMENTS };

  class Renderer {
  private:
    std::vector<Vertex>       vertices = {{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
                                          {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                                          {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                                          {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                                          {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
                                          {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
                                          {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                                          {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
                                          {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
                                          {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
                                          {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                                          {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                                          {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
                                          {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
                                          {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                                          {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                                          {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
                                          {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
                                          {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                                          {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
                                          {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
                                          {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                                          {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
                                          {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
                                          {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                                          {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
                                          {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},
                                          {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}};
    std::vector<unsigned int> indices;
    std::vector<glm::vec3>    cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    ecs::Registry registry;

    std::vector<Transform> transforms;
    Camera                 camera;

    glm::mat4 view;
    glm::mat4 projection;

    VertexArray vertexArray;
    Shader      shader;
    Texture     texture;

    unsigned int frameCount = 0;
    DrawMode     drawMode;

    void draw();

  public:
    Renderer();
    ~Renderer();

    static inline void clear() {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void render();

    static void keyCallback(std::unique_ptr<events::BaseEvent> &event);
  };

}