#include "app.hpp"

#include "GLFW/glfw3.h"
#include "camera_transform.hpp"
#include "event_manager.hpp"
#include "events.hpp"
#include "inputs.hpp"
#include "renderer.hpp"

App::App()
    : window(App::getScreenSize().width, App::getScreenSize().height,
             AppConfig::NAME),
      camera(registry, glm::vec3(0.0f, 0.0f, 3.0f),
             glm::vec3(0.0f, 1.0f, 0.0f)) {
  this->registry.addComponent<core::CameraTransform>(
      this->registry.createEntity(), camera.getCameraTransform());
  this->registry.getPool<core::CameraTransform>().get(0).setCameraActive(true);
  layers.emplace_back(std::make_unique<inputs::Inputs>(
      this->window, this->registry, this->stateManager));
  layers.emplace_back(std::make_unique<gl::Renderer>(
      App::getScreenSize().width, App::getScreenSize().height,
      inputs::Inputs::getScreenFov().fov, this->registry));
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::WINDOW_CLOSE_EVENT, App::windowCloseHandler);
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::MOUSE_BUTTON_EVENT, App::mouseButtonHandler);
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::WINDOW_RESIZE_EVENT, App::windowResizeHandler);
}

void App::windowCloseHandler(core::InputEvent &event) {
  core::logger::info("Shutting down application");
  App::isRunning() = false;
}

void App::mouseButtonHandler(core::InputEvent &event) {
  core::MouseButtonEvent mouseButtonEvent =
      std::get<core::MouseButtonEvent>(event.data);
  core::logger::info("Received mouse button event: {}, {}",
                     mouseButtonEvent.button,
                     static_cast<int>(mouseButtonEvent.type));
}

void App::windowResizeHandler(core::InputEvent &event) {
  core::WindowResizeEvent windowResizeEvent =
      std::get<core::WindowResizeEvent>(event.data);
  App::getScreenSize().width = windowResizeEvent.width;
  App::getScreenSize().height = windowResizeEvent.height;
}

bool &App::isRunning() {
  static bool running = true;
  return running;
}

ScreenSize &App::getScreenSize() {
  static ScreenSize screenSize{800, 600};
  return screenSize;
}

void App::run() {
  int backend = glfwGetPlatform();
  core::logger::info("backend: {} {}", backend, GLFW_PLATFORM_X11);
  float previousFrame = glfwGetTime();
  while (App::isRunning()) {
    window.pollEvents();
    core::InputEventManager::getInstance().executeEvents();
    float currentFrame = glfwGetTime();
    for (std::unique_ptr<core::Layer> &layer : layers)
      layer->onUpdate((currentFrame - previousFrame) * 1000);
    previousFrame = currentFrame;
    window.processGlfwFrame();
  }
}