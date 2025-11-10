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

namespace core {
  using Event =
      std::variant<MouseEvent, KeyEvent, WindowResizeEvent, WindowCloseEvent>;

  enum class InputEventType {
    MOUSE_EVENT,
    KEY_EVENT,
    WINDOW_RESIZE_EVENT,
    WINDOW_CLOSE_EVENT
  };

  class InputEvent {
  private:
    InputEventType type;
    Event          data;

  public:
    InputEvent(KeyEvent data);
    InputEvent(MouseEvent data);
    InputEvent(WindowResizeEvent data);
    InputEvent(WindowCloseEvent data);

    InputEventType getType();

    const Event &getData() const;

    bool operator==(InputEvent &event);

    static const std::vector<InputEventType> &getEventTypes();
  };
}