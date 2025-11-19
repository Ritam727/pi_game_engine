#pragma once

#include "mode_manager.hpp"
#include "states.hpp"

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
    const std::vector<std::string>             &keyToStringMap;

    std::string getFirstMatch(unsigned int idx, std::string current);
    std::vector<std::string> getKeysInCurrentActivation();
    bool                     currentActivationContainsKey(unsigned int key);

    StateManager();

  public:
    static StateManager &getInstance();

    void addKey(unsigned int key);
    void removeKey(unsigned int key);
    void handleDeactivations();
    void handleActivations();
  };
}