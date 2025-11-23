#include "renderer.hpp"

#include "camera_transform.hpp"
#include "gl_constants.hpp"
#include "event_manager.hpp"
#include "materials.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "registry.hpp"
#include "gl_events.hpp"
#include "resource_manager.hpp"
#include "transform.hpp"

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace gl {
  Renderer::Renderer(core::Registry &registry, core::EventManager &eventManager,
                     core::ResourceManager &resourceManager)
      : registry(registry), eventManager(eventManager) {
    std::vector<Model> backpack{
        ModelLoader::loadModels(ENGINE_PATH "/res/models/backpack/backpack.obj",
                                this->registry, resourceManager)};
    this->registerWindowResizeCallback();
    this->registerFovChangeCallback();

    this->registry.addComponent<DirectionalLight>(
        this->light.getEntityId(), glm::vec3{0.05f}, glm::vec3{0.1f},
        glm::vec3{0.2f});

    this->registry.addComponent<PointLights>(
        this->light.getEntityId(),
        std::vector<PointLight>{
            4,
            {glm::vec3{0.1f}, glm::vec3{0.2f}, glm::vec3{0.4f}}});
    for (unsigned int i = 0; i < 4; i++) {
      this->registry.getPool<PointLights>()
          .get(this->light.getEntityId())
          .lights[i]
          .position = this->pointLightPositions[i];
    }

    this->registry.addComponent<SpotLight>(
        this->light.getEntityId(), glm::vec3{0.2f}, glm::vec3{0.4f},
        glm::vec3{0.8f}, glm::cos(glm::radians(12.0f)),
        glm::cos(glm::radians(17.5f)));

    glEnable(GL_DEPTH_TEST);
  }

  void Renderer::registerWindowResizeCallback() {
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
  }

  void Renderer::registerFovChangeCallback() {
    this->eventManager.subscribe(
        Constants::FOV_CHANGE_TOPIC, [&](core::IEventPtr &event) {
          FovChangeEvent fovChangeEvent =
              (static_cast<core::Event<FovChangeEvent> *>(event.get()))->data;
          this->renderState.fov = fovChangeEvent.fov;
        });
  }

  void Renderer::bindLights() {
    PointLights &pointLights =
        this->registry.getPool<PointLights>().get(this->light.getEntityId());
    this->shader.set<int>("pointLightCount", pointLights.lights.size());
    for (unsigned int i = 0; i < pointLights.lights.size(); i++)
      this->shader.set<PointLight &>("pointLights[" + std::to_string(i) + "]",
                                     pointLights.lights[i]);

    this->shader.set<DirectionalLight &>(
        "directionalLight", this->registry.getPool<DirectionalLight>().get(
                                this->light.getEntityId()));

    this->shader.set<SpotLight &>(
        "spotLight",
        this->registry.getPool<SpotLight>().get(this->light.getEntityId()));
  }

  void Renderer::drawObjects(float ts) {
    std::vector<core::Entity> &entities =
        this->registry.getPool<Mesh>().getEntities();
    core::SparseSet<core::Entity, Mesh> &meshes =
        this->registry.getPool<Mesh>();
    core::SparseSet<core::Entity, core::Transform> &transforms =
        this->registry.getPool<core::Transform>();
    core::SparseSet<core::Entity, Material> &materials =
        this->registry.getPool<Material>();

    unsigned int i = 0;
    for (core::Entity &entity : entities) {
      core::Transform &transform = transforms.get(entity);
      this->shader.set<glm::mat4>("model", transform.getModelMatrix());

      Material &material = materials.get(entity);
      this->shader.set<Material &>("material", material);

      Mesh &mesh = meshes.get(entity);
      mesh.draw();
    }
  }

  void Renderer::onUpdate(float ts) {
    this->clear();
    this->shader.use();

    for (core::CameraTransform &cameraTransform :
         registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.isCameraActive()) {
        glm::mat4 view = cameraTransform.getViewMatrix();
        this->shader.set<glm::mat4>("view", view);
        this->shader.set<glm::vec3>("viewerPos", cameraTransform.getPosition());
        this->registry.getPool<SpotLight>()
            .get(this->light.getEntityId())
            .position = cameraTransform.getPosition();
        this->registry.getPool<SpotLight>()
            .get(this->light.getEntityId())
            .direction = cameraTransform.getForwardDirection();
      }
    }

    this->bindLights();

    glm::mat4 projection = glm::perspective(
        glm::radians(this->renderState.fov),
        (float) this->renderState.width / (float) this->renderState.height,
        0.1f, 100.0f);
    this->shader.set<glm::mat4>("projection", projection);

    this->drawObjects(ts);
  }
}