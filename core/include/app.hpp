#pragma once

#include "renderer.hpp"
#include "window.hpp"

#include "event_manager.hpp"

#include <thread>

namespace core {
  class App {
  public:
    App();
    ~App();

    void run();

    static void windowCloseCallback(std::unique_ptr<events::BaseEvent> &event);
    static void eventThreadExecutor();

  private:
    Window   window;
    Renderer renderer;

    static bool running;
  };
}