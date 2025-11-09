#pragma once

#include <variant>
#include <vector>

namespace core {
  class WindowResizeEvent {
  private:
    int width;
    int height;

  public:
    WindowResizeEvent(int width, int height);

    int getWidth() const;
    int getHeight() const;

    bool operator==(const WindowResizeEvent &event) const;
  };

  class WindowCloseEvent {
  public:
    bool operator==(const WindowCloseEvent &event) const;
  };
}

namespace core {
  class KeyEvent {
  public:
    enum class Type { KEY_ENTER };

  private:
    Type type;

  public:
    KeyEvent(Type type);
    ~KeyEvent();

    Type getType() const;

    bool operator==(const KeyEvent &event) const;
  };
}

namespace core {
  class MouseEvent {
  public:
    bool operator==(const MouseEvent &event) const;
  };
}

#define BASIC_EVENTS KeyEvent, MouseEvent, WindowResizeEvent, WindowCloseEvent

namespace core {
  enum class BasicEventType {
    KEY_EVENT,
    MOUSE_EVENT,
    WINDOW_RESIZE_EVENT,
    WINDOW_CLOSE_EVENT
  };

  class BasicEvent {
  private:
    BasicEventType             type;
    std::variant<BASIC_EVENTS> data;

  public:
    BasicEvent(KeyEvent data);
    BasicEvent(MouseEvent data);
    BasicEvent(WindowResizeEvent data);
    BasicEvent(WindowCloseEvent data);

    BasicEventType getType();

    const std::variant<BASIC_EVENTS> &getData() const;

    bool operator==(BasicEvent &event);

    static const std::vector<BasicEventType> &getEventTypes();
  };
}