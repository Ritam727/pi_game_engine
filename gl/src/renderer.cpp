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
      this->registry.addComponent<TextureMaterial>(
          entity, files, glm::vec3{1.0f, 1.0f, 1.0f}, 32.0f);
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

    this->shader.set<int>("pointLightCount", pointLights.size());
    for (unsigned int i = 0; i < pointLights.size(); i++) {
      this->shader.set<glm::vec3>("pointLights[" + std::to_string(i) +
                                      "].position",
                                  pointLights[i].position);
      this->shader.set<glm::vec3>("pointLights[" + std::to_string(i) +
                                      "].ambient",
                                  pointLights[i].ambient);
      this->shader.set<glm::vec3>("pointLights[" + std::to_string(i) +
                                      "].diffuse",
                                  pointLights[i].diffuse);
      this->shader.set<glm::vec3>("pointLights[" + std::to_string(i) +
                                      "].specular",
                                  pointLights[i].specular);
      this->shader.set<float>("pointLights[" + std::to_string(i) + "].constant",
                              pointLights[i].constant);
      this->shader.set<float>("pointLights[" + std::to_string(i) + "].linear",
                              pointLights[i].linear);
      this->shader.set<float>("pointLights[" + std::to_string(i) +
                                  "].quadratic",
                              pointLights[i].quadratic);
    }

    this->shader.set<glm::vec3>("directionalLight.direction",
                                directionalLight.direction);
    this->shader.set<glm::vec3>("directionalLight.ambient",
                                directionalLight.ambient);
    this->shader.set<glm::vec3>("directionalLight.diffuse",
                                directionalLight.diffuse);
    this->shader.set<glm::vec3>("directionalLight.specular",
                                directionalLight.specular);

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

    this->shader.set<glm::vec3>("spotLight.ambient", spotLight.ambient);
    this->shader.set<glm::vec3>("spotLight.diffuse", spotLight.diffuse);
    this->shader.set<glm::vec3>("spotLight.specular", spotLight.specular);
    this->shader.set<glm::vec3>("spotLight.position", spotLight.position);
    this->shader.set<glm::vec3>("spotLight.direction", spotLight.direction);
    this->shader.set<float>("spotLight.innerCutOff", spotLight.innerCutOff);
    this->shader.set<float>("spotLight.outerCutOff", spotLight.outerCutOff);
    this->shader.set<float>("spotLight.constant", spotLight.constant);
    this->shader.set<float>("spotLight.linear", spotLight.linear);
    this->shader.set<float>("spotLight.quadratic", spotLight.quadratic);

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
      material.textures.bind();
      this->shader.set<int>("textureMaterial.diffuse", 0);
      this->shader.set<int>("textureMaterial.specular", 1);
      this->shader.set<float>("textureMaterial.shininess", material.shininess);

      this->draw();
    }
    this->frameCount = (this->frameCount + 1) % 36000;
    this->cameraAngle += angle;
  }
}