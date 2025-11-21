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
    std::unordered_map<
        std::string,
        std::function<void(std::string &, std::array<std::string, 2> &)>>
        constructorMap{};
    std::unordered_map<std::string,
                       std::pair<std::function<void()>, std::function<void()>>>
                                    postFuncMap{};
    const std::vector<std::string> &keyToStringMap{
        core::Window::getGlfwToKeyMapping()};

    std::string getFirstMatch(unsigned int idx, std::string current);

    std::vector<std::string> getKeysInCurrentActivation();

    bool currentActivationContainsKey(unsigned int key);

    template <core::IsIterableEnum T>
    static inline std::function<void(ModeManager &, std::function<void()>)>
        getFunction(std::string &command) {
      std::function<void(ModeManager &, std::function<void()>)> func;
      if (command.contains("inc")) {
        func = [=](ModeManager &modeManager, std::function<void()> postFunc) {
          modeManager.incrementModeFor<T>();
          postFunc();
        };
      } else if (command.contains("none")) {
        func = [](ModeManager &modeManager, std::function<void()> postFunc) {};
      } else {
        unsigned int val = StateManager::fetchValueFromCommand(command);
        func = [=](ModeManager &modeManager, std::function<void()> postFunc) {
          modeManager.setModeFor<T>(val);
          postFunc();
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

  public:
    StateManager();

    void addKey(unsigned int key);
    void removeKey(unsigned int key);
    void loadKeyMap(std::string filePath);

    template <core::IsIterableEnum T>
    static inline Activation
        createActivation(std::array<std::string, 2> &commands) {
      std::function<void(ModeManager & modeManager, std::function<void()>)>
          onPress = StateManager::getFunction<T>(commands[0]);
      std::function<void(ModeManager & modeManager, std::function<void()>)>
          onRelease = StateManager::getFunction<T>(commands[1]);
      return {onPress, onRelease, false};
    }

    template <core::IsIterableEnum T> Mode<T> *getMode() {
      return this->modeManager.getModePointer<T>();
    }

    template <core::IsIterableEnum T, typename... Args>
    void registerMode(
        Args... args, std::function<void()> postOnPress = []() {},
        std::function<void()> postOnRelease = []() {}) {
      std::string concreteName = core::getStaticType<T>();
      if (this->constructorMap.contains(concreteName)) {
        core::logger::warn("{} already registered, not registering again",
                           concreteName);
      }
      this->constructorMap[concreteName] =
          [&](std::string                &keyPress,
              std::array<std::string, 2> &keyCombination) {
            this->activations.insert(
                {keyPress, StateManager::createActivation<T>(keyCombination)});
          };
      this->postFuncMap[concreteName] = {postOnPress, postOnRelease};
      this->modeManager.registerMode<T>(args...);
    }
  };
}