#pragma once

#include "mode_manager.hpp"
#include "states.hpp"
#include "utils.hpp"
#include "window.hpp"

namespace inputs {
  class StateKeyMap {
  private:
    std::unordered_map<
        StateType, std::unordered_map<unsigned int,
                                      std::vector<std::vector<unsigned int>>>>
        stateKeyMap;

  public:
    StateKeyMap();

    std::unordered_map<unsigned int, std::vector<std::vector<unsigned int>>> &
        getInputMap(StateType type);
  };
}

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

    StateManager();

  public:
    static StateManager &getInstance();

    void addKey(unsigned int key);
    void removeKey(unsigned int key);
    void handleDeactivations();
    void handleActivations();

    template <core::IsIterableEnum T>
    static Activation createActivation(std::array<std::string, 2> &commands) {
      std::function<void(ModeManager & modeManager)> onPress =
          StateManager::getFunction<T>(commands[0]);
      std::function<void(ModeManager & modeManager)> onRelease =
          StateManager::getFunction<T>(commands[1]);
      return {onPress, onRelease, false};
    }

    template <core::IsIterableEnum T>
    static std::function<void(ModeManager &)>
        getFunction(std::string &command) {
      std::function<void(ModeManager &)> func;
      if (command.contains("inc")) {
        func = [](ModeManager &modeManager) {
          modeManager.incrementModeFor<T>();
        };
      } else if (command.contains("none")) {
        func = [](ModeManager &modeManager) {};
      } else {
        std::string cur{};
        for (unsigned int i = command.size() - 1; i >= 0; i--) {
          if (command[i] == ' ')
            break;
          cur += command[i];
        }
        std::reverse(cur.begin(), cur.end());
        unsigned int val = std::stoi(cur);
        func = [&](ModeManager &modeManager) {
          modeManager.setModeFor<T>(val);
        };
      }
      return func;
    }
  };
}