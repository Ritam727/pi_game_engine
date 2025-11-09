#include "app.hpp"

#include "event_manager.hpp"

App::App()
    : window(config.getWidth(), config.getHeight(), config.getName()),
      renderer(config.getWidth(), config.getHeight()) {
  core::EventManager::getInstance().subscribe(
      core::BasicEventType::WINDOW_CLOSE_EVENT, App::windowCloseCallback);
  core::EventManager::getInstance().subscribe(
      core::BasicEventType::WINDOW_RESIZE_EVENT, App::windowResizeCallback);
}

void App::windowCloseCallback(core::BasicEvent &event) {
  core::logger::info("Shutting down application");
  App::running = false;
}

void App::windowResizeCallback(core::BasicEvent &event) {
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
  std::thread eventThread(App::eventThreadExecutor);

  while (App::running) {
    gl::Renderer::clear();
    renderer.render();
    window.processGlfwFrame();
  }

  eventThread.join();
}

void App::eventThreadExecutor() {
  while (App::running) {
    core::EventManager::getInstance().executeEvents();
  }
}

bool      App::running = true;
AppConfig App::config;