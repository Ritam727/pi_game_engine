#include "app.hpp"

#include "GLFW/glfw3.h"
#include "camera_transform.hpp"
#include "event_manager.hpp"
#include "events.hpp"
#include "inputs.hpp"
#include "renderer.hpp"

App::App()
    : window(config.getWidth(), config.getHeight(), config.getName()),
      camera(registry, glm::vec3(0.0f, 0.0f, 3.0f),
             glm::vec3(0.0f, 1.0f, 0.0f)) {
  this->registry.addComponent<core::CameraTransform>(
      this->registry.createEntity(), camera.getCameraTransform());
  this->registry.getPool<core::CameraTransform>().get(0).setCameraActive(true);
  layers.emplace_back(std::make_unique<inputs::Inputs>(this->registry));
  layers.emplace_back(std::make_unique<gl::Renderer>(
      config.getWidth(), config.getHeight(), this->registry));
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::WINDOW_CLOSE_EVENT, App::windowCloseHandler);
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::WINDOW_RESIZE_EVENT, App::windowResizeHandler);
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::MOUSE_BUTTON_EVENT, App::mouseButtonHandler);
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::MOUSE_SCROLL_EVENT, App::mouseScrollHandler);
}

void App::windowCloseHandler(core::InputEvent &event) {
  core::logger::info("Shutting down application");
  App::running = false;
}

void App::mouseButtonHandler(core::InputEvent &event) {
  core::MouseButtonEvent mouseButtonEvent =
      std::get<core::MouseButtonEvent>(event.getData());
  core::logger::info("Received mouse button event: {}, {}",
                     mouseButtonEvent.getButton(),
                     static_cast<int>(mouseButtonEvent.getType()));
}

void App::mouseScrollHandler(core::InputEvent &event) {
  core::MouseScrollEvent mouseScrollEvent =
      std::get<core::MouseScrollEvent>(event.getData());
  core::logger::info("Received mouse scroll event: {}, {}",
                     mouseScrollEvent.getX(), mouseScrollEvent.getY());
}

void App::windowResizeHandler(core::InputEvent &event) {
  core::WindowResizeEvent windowResizeEvent =
      std::get<core::WindowResizeEvent>(event.getData());
  core::logger::info("Resizing window from ({}, {}) to ({}, {})",
                     App::config.getWidth(), App::config.getHeight(),
                     windowResizeEvent.getWidth(),
                     windowResizeEvent.getHeight());
  App::config.setWidth(windowResizeEvent.getWidth());
  App::config.setHeight(windowResizeEvent.getHeight());
}

void App::run() {
  int backend = glfwGetPlatform();
  core::logger::info("backend: {} {}", backend, GLFW_PLATFORM_X11);
  float previousFrame = glfwGetTime();
  while (App::running) {
    window.pollEvents();
    core::InputEventManager::getInstance().executeEvents();
    float currentFrame = glfwGetTime();
    for (std::unique_ptr<core::Layer> &layer : layers)
      layer->onUpdate((currentFrame - previousFrame) * 1000);
    previousFrame = currentFrame;
    window.processGlfwFrame();
  }
}

bool      App::running = true;
AppConfig App::config;