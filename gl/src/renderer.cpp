#include "renderer.hpp"

#include "camera_transform.hpp"
#include "gl_constants.hpp"
#include "event_manager.hpp"
#include "lights.hpp"
#include "materials.hpp"
#include "mesh.hpp"
#include "registry.hpp"
#include "gl_events.hpp"
#include "resource_manager.hpp"
#include "sparse_set.hpp"
#include "transform.hpp"

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace gl {
  Renderer::Renderer(core::Registry &registry, core::EventManager &eventManager,
                     core::ResourceManager &resourceManager, int initialWidth,
                     int initialHeight)
      : resourceManager(resourceManager), registry(registry),
        eventManager(eventManager),
        renderState({.width = initialWidth, .height = initialHeight}) {
    this->modelLoader.loadModels(ENGINE_PATH
                                 "/res/models/backpack/backpack.obj");
    this->registerWindowResizeCallback();
    this->registerFovChangeCallback();
    this->registerModelLoadCallback();

    this->registry.addComponent<core::LightComponent>(
        this->light.getEntityId(), core::LightType::DIRECTIONAL_LIGHT);

    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glEnable(GL_CULL_FACE));
    GL_CALL(glCullFace(GL_BACK));
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

  void Renderer::registerModelLoadCallback() {
    this->eventManager.subscribe(
        Constants::MODEL_LOAD_TOPIC, [&](core::IEventPtr &event) {
          std::string path =
              static_cast<core::Event<ModelLoadEvent> *>(event.get())
                  ->data.path;
          this->modelLoader.loadModels(path);
        });
  }

  void Renderer::bindLights() {
    core::LightComponent &component =
        this->registry.getPool<core::LightComponent>().get(
            this->light.getEntityId());
    this->shader.set<core::LightComponent &>(
        "light", this->registry.getPool<core::LightComponent>().get(
                     this->light.getEntityId()));
  }

  void Renderer::drawObjects(float ts) {
    unsigned int numEntities = this->registry.getPool<Mesh>().getNumElements();
    const std::vector<core::Entity> &entities =
        this->registry.getPool<Mesh>().getEntities();

    core::ExtendedSparseSet<core::Entity, Mesh> &meshes =
        this->registry.getPool<Mesh>();
    core::ExtendedSparseSet<core::Entity, core::Transform> &transforms =
        this->registry.getPool<core::Transform>();
    core::ExtendedSparseSet<core::Entity, core::Material> &materials =
        this->registry.getPool<core::Material>();

    for (unsigned int i = 0; i < numEntities; i++) {
      const core::Entity &entity = entities[i];

      core::Transform &transform = transforms.get(entity);
      this->shader.set<glm::mat4>("model", transform.getModelMatrix());

      core::Material &material = materials.get(entity);
      this->shader.set<core::Material &>("material", material,
                                         this->resourceManager);

      Mesh &mesh = meshes.get(entity);
      mesh.draw();
    }
  }

  void Renderer::onUpdate(float ts) {
    this->shader.use();

    for (core::CameraTransform &cameraTransform :
         registry.getPool<core::CameraTransform>().getComponents()) {
      if (cameraTransform.cameraActive) {
        glm::mat4 view = cameraTransform.getViewMatrix();
        this->shader.set<glm::mat4>("view", view);
        this->shader.set<glm::vec3>("viewerPos", cameraTransform.position);
      }
    }

    this->bindLights();

    glm::mat4 projection = glm::perspective(
        glm::radians(this->renderState.fov),
        (float) this->renderState.width / (float) this->renderState.height,
        0.1f, 2000.0f);
    this->shader.set<glm::mat4>("projection", projection);

    this->drawObjects(ts);
  }
}