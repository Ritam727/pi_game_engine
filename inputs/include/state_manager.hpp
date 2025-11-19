#pragma once

#include "mode_manager.hpp"
#include "states.hpp"
#include "utils.hpp"
#include "window.hpp"

namespace inputs {
  class StateManager {
  private:
    InputState                                  inputState{};
    ModeManager                                 modeManager{};
    std::unordered_map<std::string, Activation> activations{};
    const std::vector<std::string>             &keyToStringMap{
        core::Window::getGlfwToKeyMapping()};

    std::string getFirstMatch(unsigned int idx, std::string current);

    std::vector<std::string> getKeysInCurrentActivation();

    bool currentActivationContainsKey(unsigned int key);

    template <core::IsIterableEnum T>
    static inline std::function<void(ModeManager &)>
        getFunction(std::string &command) {
      std::function<void(ModeManager &)> func;
      if (command.contains("inc")) {
        func = [](ModeManager &modeManager) {
          modeManager.incrementModeFor<T>();
        };
      } else if (command.contains("none")) {
        func = [](ModeManager &modeManager) {};
      } else {
        unsigned int val = StateManager::fetchValueFromCommand(command);
        func = [=](ModeManager &modeManager) {
          modeManager.setModeFor<T>(val);
        };
      }
      return func;
    }

    static inline unsigned int fetchValueFromCommand(std::string &command) {
      std::string cur{};
      for (unsigned int i = command.size() - 1; i >= 0; i--) {
        if (command[i] == ' ')
          break;
        cur += command[i];
      }
      std::reverse(cur.begin(), cur.end());
      unsigned int val = std::stoi(cur);
      return val;
    }

    StateManager();

  public:
    static StateManager &getInstance();

    void addKey(unsigned int key);
    void removeKey(unsigned int key);

    template <core::IsIterableEnum T>
    static inline Activation
        createActivation(std::array<std::string, 2> &commands) {
      std::function<void(ModeManager & modeManager)> onPress =
          StateManager::getFunction<T>(commands[0]);
      std::function<void(ModeManager & modeManager)> onRelease =
          StateManager::getFunction<T>(commands[1]);
      return {onPress, onRelease, false};
    }

    template <core::IsIterableEnum T> Mode<T> *getMode() {
      return this->modeManager.getModePointer<T>();
    }

    template <core::IsIterableEnum T, typename... Args>
    void registerMode(Args... args) {
      this->modeManager.registerMode<T>(args...);
    }
  };
}