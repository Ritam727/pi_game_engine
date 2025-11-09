#pragma once

#include "event.hpp"
#include "GLFW/glfw3.h"

namespace core {
  class WindowEvent : public events::BaseEvent {
  public:
    virtual bool equals(std::unique_ptr<BaseEvent> &event) override;
  };

  class WindowResizeEvent : public WindowEvent {
  private:
    int width;
    int height;

  public:
    WindowResizeEvent(int width, int height);

    int getWidth() const;
    int getHeight() const;

    virtual bool equals(std::unique_ptr<BaseEvent> &event) override;
  };

  class WindowCloseEvent : public WindowEvent {};

  class KeyEvent : public events::BaseEvent {
  public:
    enum class Type { KEY_ENTER = GLFW_KEY_ENTER };

  private:
    Type type;

  public:
    KeyEvent(Type type);
    ~KeyEvent();

    Type getType();

    bool equals(std::unique_ptr<BaseEvent> &event) override;
  };
}