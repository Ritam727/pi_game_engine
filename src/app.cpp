#include "app.hpp"

#include "event_manager.hpp"
#include "events.hpp"

App::App()
    : window(config.getWidth(), config.getHeight(), config.getName()),
      renderer(config.getWidth(), config.getHeight()) {
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::WINDOW_CLOSE_EVENT, App::windowCloseHandler);
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::WINDOW_RESIZE_EVENT, App::windowResizeHandler);
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::MOUSE_BUTTON_EVENT, App::mouseButtonHandler);
  core::InputEventManager::getInstance().subscribe(
      core::InputEventType::MOUSE_MOVEMENT_EVENT, App::mouseMovementHandler);
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

void App::mouseMovementHandler(core::InputEvent &event) {
  core::MouseMovementEvent mouseMovementEvent =
      std::get<core::MouseMovementEvent>(event.getData());
  core::logger::info("Received mouse movement event: {}, {}",
                     mouseMovementEvent.getX(), mouseMovementEvent.getY());
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
  while (App::running) {
    core::InputEventManager::getInstance().executeEvents();
    gl::Renderer::clear();
    renderer.render();
    window.processGlfwFrame();
  }
}

bool      App::running = true;
AppConfig App::config;