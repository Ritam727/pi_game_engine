#include "renderer.hpp"

#include "camera_transform.hpp"
#include "gl_constants.hpp"
#include "event_manager.hpp"
#include "materials.hpp"
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
      this->registry.addComponent<core::Transform>(entity);
      registry.getPool<core::Transform>()
          .get(registry.getLastEntity())
          .setPosition(cubePositions[i]);
      std::vector<std::string> files{ENGINE_PATH "/res/textures/container2.png",
                                     ENGINE_PATH
                                     "/res/textures/container2_specular.png"};
      this->registry.addComponent<TextureMaterial>(entity, files[0], files[1],
                                                   32.0f);
    }

    this->registry.addComponent<DirectionalLight>(
        this->light.getEntityId(), glm::vec3{0.1f}, glm::vec3{0.2f},
        glm::vec3{0.4f});

    this->registry.addComponent<SpotLight>(
        this->light.getEntityId(), glm::vec3{0.1f}, glm::vec3{0.2f},
        glm::vec3{0.4f}, glm::cos(glm::radians(12.0f)),
        glm::cos(glm::radians(17.5f)));

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

    this->shader.set<int>("pointLightCount", 0);
    // for (unsigned int i = 0; i < pointLights.size(); i++) {
    //   this->shader.set<PointLight &>("pointLights[" + std::to_string(i) +
    //   "]",
    //                                  pointLights[i]);
    // }

    this->shader.set<DirectionalLight &>(
        "directionalLight", this->registry.getPool<DirectionalLight>().get(
                                this->light.getEntityId()));

    SpotLight &spotLight =
        this->registry.getPool<SpotLight>().get(this->light.getEntityId());
    for (core::CameraTransform &cameraTransform :
         registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        glm::mat4 view = cameraTransform.getViewMatrix();
        this->shader.set<glm::mat4>("view", view);
        this->shader.set<glm::vec3>("viewerPos", cameraTransform.getPosition());
        spotLight.position = cameraTransform.getPosition();
        spotLight.direction = cameraTransform.getForwardDirection();
      }
    }

    this->shader.set<SpotLight &>("spotLight", spotLight);

    glm::mat4 projection = glm::perspective(
        glm::radians(this->renderState.fov),
        (float) this->renderState.width / (float) this->renderState.height,
        0.1f, 100.0f);
    this->shader.set<glm::mat4>("projection", projection);

    this->vertexArray.bind();
    float angle = ts * Constants::SPEED_SCALAR;
    core::SparseSet<core::Entity, TextureMaterial> &materialPool =
        registry.getPool<TextureMaterial>();
    core::SparseSet<core::Entity, core::Transform> &transformPool =
        registry.getPool<core::Transform>();
    for (core::Entity &entity : materialPool.getEntities()) {
      core::Transform &transform = transformPool.get(entity);
      transform.updateRotation(glm::vec3(angle));
      glm::mat4 model = transform.getModelMatrix();
      this->shader.set<glm::mat4>("model", model);

      TextureMaterial &material = materialPool.get(entity);
      this->shader.set<TextureMaterial &>("textureMaterial", material);

      this->draw();
    }
    this->frameCount = (this->frameCount + 1) % 36000;
    this->cameraAngle += angle;
  }
}