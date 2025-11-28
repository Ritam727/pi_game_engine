#include "app.hpp"

#include "GLFW/glfw3.h"
#include "camera_transform.hpp"
#include "constants.hpp"
#include "event_manager.hpp"
#include "renderer.hpp"
#include "ui.hpp"

App::App() {
  this->layers.pushLayer<gl::Renderer>(
      this->registry, this->eventManager, this->resourceManager,
      this->screenSize.width, this->screenSize.height);
  this->layers.pushLayer<ui::UI>(this->window, this->registry);

  this->eventManager.subscribe(
      core::Constants::WINDOW_CLOSE_TOPIC, [&](core::IEventPtr &event) {
        core::logger::info("Shutting down application");
        this->running = false;
      });

  this->registry.getPool<core::CameraTransform>().get(0).setCameraActive(true);
}

void App::run() {
  std::thread eventThread([&]() {
    while (this->running) {
      this->eventManager.executeEvents(nonMainThreadTopics);
    }
  });

  float previousFrame = glfwGetTime();
  while (this->running) {
    window.pollEvents();
    this->eventManager.executeEvents(mainThreadTopics);
    float currentFrame = glfwGetTime();
    float deltaTime = (currentFrame - previousFrame) * 1000.0f;
    gl::Renderer::clear();
    this->layers.onUpdate(deltaTime);
    this->layers.postUpdate();
    previousFrame = currentFrame;
    window.processGlfwFrame();
  }

  eventThread.join();
}