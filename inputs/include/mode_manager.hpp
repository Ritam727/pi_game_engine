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
      if (this->isRegistered<T>()) {
        core::logger::warn("{} is already registered, not registering again",
                           GET_STATIC_TYPE(T));
        return;
      }
      std::string concreteName = GET_STATIC_TYPE(T);
      this->modeMap[concreteName] =
          std::make_unique<Mode<T>>(std::forward(args)...);
    }

    template <core::IsIterableEnum T> bool isRegistered() {
      std::string concreteName = GET_STATIC_TYPE(T);
      return this->modeMap.contains(concreteName);
    }

    template <core::IsIterableEnum T> Mode<T> *getModePointer() {
      std::string concreteName = GET_STATIC_TYPE(T);
      Mode<T> *mode = static_cast<Mode<T> *>(this->modeMap[concreteName].get());
      return mode;
    }

    template <core::IsIterableEnum T> void setModeFor(unsigned int val) {
      if (this->isRegistered<T>()) {
        Mode<T> *mode = this->getModePointer<T>();
        mode->setMode(val);
      }
    }

    template <core::IsIterableEnum T> void incrementModeFor() {
      if (this->isRegistered<T>()) {
        Mode<T> *mode = this->getModePointer<T>();
        mode->incrementMode();
      }
    }
  };
}