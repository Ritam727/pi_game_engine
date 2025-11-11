#include "renderer.hpp"
#include "events.hpp"
#include "utils.hpp"

#include "event_manager.hpp"

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace gl {
  Renderer::Renderer(int &width, int &height)
      : width(width), height(height), vertexArray(vertices, indices),
        shader(ENGINE_PATH "/res/shaders/shader.vert",
               ENGINE_PATH "/res/shaders/shader.frag"),
        texture({ENGINE_PATH "/res/textures/container.jpg",
                 ENGINE_PATH "/res/textures/awesomeface.png"}),
        camera(registry, glm::vec3(0.0f, 0.0f, 3.0f),
               glm::vec3(0.0f, 1.0f, 0.0f)) {
    this->registry.addComponent<CameraTransform>(this->registry.createEntity(),
                                                 camera.getCameraTransform());
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::KEY_EVENT, Renderer::keyCallback);
    core::InputEventManager::getInstance().subscribe(
        core::InputEventType::WINDOW_RESIZE_EVENT,
        Renderer::windowResizeCallback);
    for (int i = 0; i < 10; i++) {
      transforms.emplace_back(registry.createEntity(), registry);
      this->registry.addComponent<Transform>(this->registry.createEntity(),
                                             transforms.back());
      registry.getPool<Transform>()
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
    this->clear();
    this->shader.use();
    this->texture.bind();
    for (unsigned int i = 0; i < this->texture.getNumTextures(); i++) {
      const std::string textureName = "ourTexture" + std::to_string(i + 1);
      this->shader.set<int>(textureName, i);
    }

    CameraTransform &cameraTransform =
        registry.getPool<CameraTransform>().get(0);
    float radius = 10.0f;
    cameraTransform.setPosition(glm::vec3(
        radius * glm::sin(glm::radians((float) this->cameraAngle)), 0.0f,
        radius * glm::cos(glm::radians((float) this->cameraAngle))));
    glm::mat4 view = cameraTransform.getViewMatrix();
    this->shader.set<glm::mat4>("view", view);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float) this->width / (float) this->height, 0.1f,
        100.0f);
    this->shader.set<glm::mat4>("projection", projection);

    this->vertexArray.bind();
    float                      angle = ts * ((float) 60 / (float) 1000);
    ecs::SparseSet<Transform> &transformPool = registry.getPool<Transform>();
    for (Transform &transform : transformPool.getComponents()) {
      transform.updateRotation(glm::vec3(angle));
      glm::mat4 model = transform.getModelMatrix();
      this->shader.set<glm::mat4>("model", model);
      this->draw();
    }
    this->frameCount = (this->frameCount + 1) % 36000;
    this->cameraAngle += angle;
  }

  void Renderer::keyCallback(core::InputEvent &event) {
    core::KeyEvent keyEvent = std::get<core::KeyEvent>(event.getData());
    core::logger::info("Received key press event: {} {}", keyEvent.getKey(),
                       static_cast<int>(keyEvent.getType()));
  }

  void Renderer::windowResizeCallback(core::InputEvent &event) {
    core::WindowResizeEvent windowResizeEvent =
        std::get<core::WindowResizeEvent>(event.getData());
    glViewport(0, 0, windowResizeEvent.getWidth(),
               windowResizeEvent.getHeight());
  }
}