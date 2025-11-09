#pragma once

#include "renderer.hpp"
#include "window.hpp"

#include "app_config.hpp"

#include "event_manager.hpp"

#include <thread>

class App {
private:
  core::Window   window;
  core::Renderer renderer;

  static AppConfig config;
  static bool      running;

public:
  App();

  void run();

  static void windowCloseCallback(std::unique_ptr<events::BaseEvent> &event);
  static void windowResizeCallback(std::unique_ptr<events::BaseEvent> &event);
  static void eventThreadExecutor();
};