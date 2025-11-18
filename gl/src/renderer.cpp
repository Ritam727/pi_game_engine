#include "renderer.hpp"

#include "camera_transform.hpp"
#include "gl_constants.hpp"
#include "event_manager.hpp"
#include "registry.hpp"
#include "gl_events.hpp"

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace gl {
  static RenderState renderState{};

  static void windowResizeCallback(std::unique_ptr<core::BaseEvent> &event) {
    core::WindowResizeEvent *windowResizeEvent =
        static_cast<core::WindowResizeEvent *>(event.get());
    renderState.width = windowResizeEvent->width;
    renderState.height = windowResizeEvent->height;
    renderState.windowResized = true;
  }

  static void fovChangeCallback(std::unique_ptr<core::BaseEvent> &event) {
    FovChangeEvent *fovChangeEvent = static_cast<FovChangeEvent *>(event.get());
    renderState.fov = fovChangeEvent->fov;
  }
}

namespace gl {
  Renderer::Renderer(core::Registry &registry)
      : vertexArray(vertices, indices),
        shader(ENGINE_PATH "/res/shaders/shader.vert",
               ENGINE_PATH "/res/shaders/shader.frag"),
        texture({ENGINE_PATH "/res/textures/container.jpg",
                 ENGINE_PATH "/res/textures/awesomeface.png"}),
        registry(registry) {
    core::EventManager::getInstance().subscribe(Constants::WINDOW_RESIZE_TOPIC,
                                                windowResizeCallback);
    core::EventManager::getInstance().subscribe(Constants::FOV_CHANGE_TOPIC,
                                                fovChangeCallback);

    for (int i = 0; i < 10; i++) {
      this->registry.addComponent<core::Transform>(
          this->registry.createEntity(), core::Transform{});
      registry.getPool<core::Transform>()
          .get(registry.getLastEntity())
          .setPosition(cubePositions[i]);
    }

    glEnable(GL_DEPTH_TEST);
    if (indices.size() == 0)
      drawMode = DrawMode::TRIANGLES;
    else
      drawMode = DrawMode::ELEMENTS;
  }

  void Renderer::draw() {
    switch (drawMode) {
    case DrawMode::TRIANGLES:
      GL_CALL(glDrawArrays(GL_TRIANGLES, 0, vertices.size()));
      break;
    case DrawMode::ELEMENTS:
      GL_CALL(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
      break;
    }
  }

  void Renderer::onUpdate(float ts) {
    if (renderState.windowResized) {
      GL_CALL(glViewport(0, 0, renderState.width, renderState.height));
      renderState.windowResized = false;
    }
    this->clear();
    this->shader.use();
    this->texture.bind();
    for (unsigned int i = 0; i < this->texture.getNumTextures(); i++) {
      const std::string textureName = "ourTexture" + std::to_string(i + 1);
      this->shader.set<int>(textureName, i);
    }

    for (core::CameraTransform &cameraTransform :
         registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        glm::mat4 view = cameraTransform.getViewMatrix();
        this->shader.set<glm::mat4>("view", view);
      }
    }

    glm::mat4 projection = glm::perspective(
        glm::radians(renderState.fov),
        (float) renderState.width / (float) renderState.height, 0.1f, 100.0f);
    this->shader.set<glm::mat4>("projection", projection);

    this->vertexArray.bind();
    float angle = ts * Constants::SPEED_SCALAR;
    core::SparseSet<core::Entity, core::Transform> &transformPool =
        registry.getPool<core::Transform>();
    for (core::Transform &transform : transformPool.getComponents()) {
      transform.updateRotation(glm::vec3(angle));
      glm::mat4 model = transform.getModelMatrix();
      this->shader.set<glm::mat4>("model", model);
      this->draw();
    }
    this->frameCount = (this->frameCount + 1) % 36000;
    this->cameraAngle += angle;
  }
}