#pragma once

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
    InputState                                                     inputState{};
    std::unordered_map<std::string, std::pair<unsigned int, bool>> activations{
        {"_CONTROL_ALT_K", {0, 0}},
        {"_CONTROL_SHIFT_P", {1, 0}},
        {"_CONTROL_ALT_MOUSELEFT", {2, 0}}};
    const std::vector<std::string> &keyToStringMap;

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