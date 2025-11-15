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