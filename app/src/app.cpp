#include "app.hpp"

#include "GLFW/glfw3.h"
#include "camera_transform.hpp"
#include "constants.hpp"
#include "event_manager.hpp"
#include "events.hpp"
#include "inputs.hpp"
#include "renderer.hpp"

#include "app_config.hpp"

App::App()
    : window(App::getScreenSize().width, App::getScreenSize().height,
             AppConfig::NAME),
      camera(registry, glm::vec3(0.0f, 0.0f, 3.0f),
             glm::vec3(0.0f, 1.0f, 0.0f)) {
  layers.emplace_back(std::make_unique<inputs::Inputs>(
      this->window, this->registry, this->stateManager));
  layers.emplace_back(std::make_unique<gl::Renderer>(this->registry));

  core::EventManager::getInstance().subscribe(
      core::Constants::WINDOW_CLOSE_TOPIC, App::windowCloseHandler);

  this->registry.addComponent<core::CameraTransform>(
      this->registry.createEntity(), camera.getCameraTransform());
  this->registry.getPool<core::CameraTransform>().get(0).setCameraActive(true);
}

void App::windowCloseHandler(std::unique_ptr<core::BaseEvent> &event) {
  core::logger::info("Shutting down application");
  App::isRunning() = false;
}

bool &App::isRunning() {
  static bool running = true;
  return running;
}

ScreenSize &App::getScreenSize() {
  static ScreenSize screenSize{800, 600};
  return screenSize;
}

void App::eventManagerThread() {
  while (App::isRunning()) {
    core::logger::info("{}", nonMainThreadTopics.size());
    core::EventManager::getInstance().executeEvents(nonMainThreadTopics);
  }
}

void App::run() {
  int         backend = glfwGetPlatform();
  float       previousFrame = glfwGetTime();
  std::thread eventThread(App::eventManagerThread);

  while (App::isRunning()) {
    window.pollEvents();
    core::logger::info("{}", mainThreadTopics.size());
    core::EventManager::getInstance().executeEvents(mainThreadTopics);
    float currentFrame = glfwGetTime();
    for (std::unique_ptr<core::Layer> &layer : layers)
      layer->onUpdate((currentFrame - previousFrame) * 1000);
    previousFrame = currentFrame;
    window.processGlfwFrame();
  }

  eventThread.join();
}