#include "app.hpp"

#include "GLFW/glfw3.h"
#include "camera_transform.hpp"
#include "constants.hpp"
#include "event_manager.hpp"
#include "renderer.hpp"

App::App() {
  layers.emplace_back(
      std::make_unique<gl::Renderer>(this->registry, eventManager));

  this->eventManager.subscribe(
      core::Constants::WINDOW_CLOSE_TOPIC, [&](core::IEventPtr &event) {
        core::logger::info("Shutting down application");
        this->running = false;
      });

  this->registry.getPool<core::CameraTransform>().get(0).setCameraActive(true);
}

ScreenSize &App::getScreenSize() {
  static ScreenSize screenSize{800, 600};
  return screenSize;
}

void App::run() {
  int         backend = glfwGetPlatform();
  float       previousFrame = glfwGetTime();
  std::thread eventThread([&]() {
    while (this->running) {
      this->eventManager.executeEvents(nonMainThreadTopics);
    }
  });

  while (this->running) {
    window.pollEvents();
    this->eventManager.executeEvents(mainThreadTopics);
    float currentFrame = glfwGetTime();
    for (std::unique_ptr<core::Layer> &layer : layers)
      layer->onUpdate((currentFrame - previousFrame) * 1000);
    previousFrame = currentFrame;
    window.processGlfwFrame();
  }

  eventThread.join();
}