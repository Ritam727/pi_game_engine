#pragma once

#include "base_component.hpp"

namespace core {
  struct Selectable : public core::BaseComponent {
    bool selected{false};

    Selectable() {}

    Selectable(bool selected) : selected(selected) {}

    void clearComponent() override {}
  };
}