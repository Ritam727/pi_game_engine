#pragma once

#include "modes.hpp"
#include "utils.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <cxxabi.h>

namespace inputs {
  class ModeManager {
  private:
    std::unordered_map<std::string, std::unique_ptr<IMode>> modeMap{};

  public:
    std::unique_ptr<IMode> &getMode(std::string mode);

    template <core::IsIterableEnum T, typename... Args>
    void registerMode(Args... args) {
      std::string concreteName = GET_STATIC_TYPE(T);
      if (this->modeMap.contains(concreteName)) {
        core::logger::warn("{} is already registered, not registering again",
                           concreteName);
        return;
      }
      this->modeMap[concreteName] =
          std::make_unique<Mode<T>>(std::forward(args)...);
    }

    template <core::IsIterableEnum T> Mode<T> *getModePointer() {
      std::string concreteName = GET_STATIC_TYPE(T);
      if (!this->modeMap.contains(concreteName)) {
        core::logger::warn("{} is not registered", concreteName);
        return;
      }
      Mode<T> *mode = static_cast<Mode<T> *>(this->modeMap[concreteName].get());
      return mode;
    }

    template <core::IsIterableEnum T> void setModeFor(unsigned int val) {
      Mode<T> *mode = this->getModePointer<T>();
      mode->setMode(val);
    }

    template <core::IsIterableEnum T> void incrementModeFor() {
      Mode<T> *mode = this->getModePointer<T>();
      mode->incrementMode();
    }
  };
}