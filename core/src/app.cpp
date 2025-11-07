#include "app.hpp"
#include "event_manager.hpp"
#include "events.hpp"

namespace core {
  App::App() : window(800, 600, "LearnOpenGL"), renderer() {
    events::EventManager::getInstance().subscribe<WindowCloseEvent>(
        std::make_unique<events::EventHandle>(App::windowCloseCallback));
  }

  App::~App() {}

  void App::windowCloseCallback(std::unique_ptr<events::BaseEvent> &event) {
    App::running = false;
  }

  void App::run() {
    std::thread eventThread(App::eventThreadExecutor);

    while (App::running) {
      Renderer::clear();
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

  bool App::running = true;
}