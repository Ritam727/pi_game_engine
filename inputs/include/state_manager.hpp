#pragma once

#include "states.hpp"

namespace inputs {
  class StateManager {
  private:
    State state;
    std::unordered_map<
        StateType, std::unordered_map<unsigned int,
                                      std::vector<std::vector<unsigned int>>>>
        inputStateMap;

  public:
    StateManager();

    State &getState();

    std::unordered_map<unsigned int, std::vector<std::vector<unsigned int>>> &
        getInputMap(StateType type);
  };
}