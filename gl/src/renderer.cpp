#include "renderer.hpp"

#include "camera_transform.hpp"
#include "gl_constants.hpp"
#include "event_manager.hpp"
#include "registry.hpp"
#include "gl_events.hpp"
#include "transform.hpp"

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace gl {
  Renderer::Renderer(core::Registry &registry, core::EventManager &eventManager)
      : registry(registry), eventManager(eventManager) {
    this->eventManager.subscribe(
        Constants::WINDOW_RESIZE_TOPIC, [&](core::IEventPtr &event) {
          core::WindowResizeEvent windowResizeEvent =
              (static_cast<core::Event<core::WindowResizeEvent> *>(event.get()))
                  ->data;
          this->renderState.width = windowResizeEvent.width;
          this->renderState.height = windowResizeEvent.height;
          GL_CALL(glViewport(0, 0, windowResizeEvent.width,
                             windowResizeEvent.height));
        });
    this->eventManager.subscribe(
        Constants::FOV_CHANGE_TOPIC, [&](core::IEventPtr &event) {
          FovChangeEvent fovChangeEvent =
              (static_cast<core::Event<FovChangeEvent> *>(event.get()))->data;
          this->renderState.fov = fovChangeEvent.fov;
        });

    for (int i = 0; i < 10; i++) {
      core::Entity entity = this->registry.createEntity();
      this->registry.addComponent<core::Transform>(entity, core::Transform{});
      registry.getPool<core::Transform>()
          .get(registry.getLastEntity())
          .setPosition(cubePositions[i]);
      this->registry.addComponent<Material>(entity, materials[i]);
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

    this->shader.set<glm::vec3>("basicLight.position", basicLight.position);
    this->shader.set<glm::vec3>("basicLight.ambient", basicLight.ambient);
    this->shader.set<glm::vec3>("basicLight.diffuse", basicLight.diffuse);
    this->shader.set<glm::vec3>("basicLight.specular", basicLight.specular);

    for (core::CameraTransform &cameraTransform :
         registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        glm::mat4 view = cameraTransform.getViewMatrix();
        this->shader.set<glm::mat4>("view", view);
        this->shader.set<glm::vec3>("viewerPos", cameraTransform.getPosition());
      }
    }

    glm::mat4 projection = glm::perspective(
        glm::radians(this->renderState.fov),
        (float) this->renderState.width / (float) this->renderState.height,
        0.1f, 100.0f);
    this->shader.set<glm::mat4>("projection", projection);

    this->vertexArray.bind();
    float angle = ts * Constants::SPEED_SCALAR;
    core::SparseSet<core::Entity, Material> &materialPool =
        registry.getPool<Material>();
    core::SparseSet<core::Entity, core::Transform> &transformPool =
        registry.getPool<core::Transform>();
    for (core::Entity &entity : materialPool.getEntities()) {
      core::Transform &transform = transformPool.get(entity);
      transform.updateRotation(glm::vec3(angle));
      glm::mat4 model = transform.getModelMatrix();

      this->shader.set<glm::mat4>("model", model);
      Material &material = materialPool.get(entity);
      this->shader.set<glm::vec3>("material.ambient", material.ambient);
      this->shader.set<glm::vec3>("material.diffuse", material.diffuse);
      this->shader.set<glm::vec3>("material.specular", material.specular);
      this->shader.set<float>("material.shininess", material.shininess);

      this->draw();
    }
    this->frameCount = (this->frameCount + 1) % 36000;
    this->cameraAngle += angle;
  }
}