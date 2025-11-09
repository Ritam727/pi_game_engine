#include "app.hpp"
#include "event_manager.hpp"
#include "events.hpp"

App::App()
    : window(config.getWidth(), config.getHeight(), config.getName()),
      renderer(config.getWidth(), config.getHeight()) {
  events::EventManager::getInstance().subscribe<core::WindowCloseEvent>(
      std::make_unique<events::EventHandle>(App::windowCloseCallback));
  events::EventManager::getInstance().subscribe<core::WindowResizeEvent>(
      std::make_unique<events::EventHandle>(App::windowResizeCallback));
}

void App::windowCloseCallback(std::unique_ptr<events::BaseEvent> &event) {
  App::running = false;
}

void App::windowResizeCallback(std::unique_ptr<events::BaseEvent> &event) {
  core::WindowResizeEvent *windowResizeEvent =
      (core::WindowResizeEvent *) event.get();
  App::config.setWidth(windowResizeEvent->getWidth());
  App::config.setHeight(windowResizeEvent->getHeight());
}

void App::run() {
  std::thread eventThread(App::eventThreadExecutor);

  while (App::running) {
    core::Renderer::clear();
    renderer.render();
    window.processGlfwFrame();
  }

  eventThread.join();
}

void App::eventThreadExecutor() {
  while (App::running) {
    events::EventManager::getInstance().executeEvents();
  }
}

bool      App::running = true;
AppConfig App::config;