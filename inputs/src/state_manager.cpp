#include "state_manager.hpp"

#include "nlohmann/json.hpp"
#include "states.hpp"
#include <fstream>
#include <string>

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace inputs {
  StateKeyMap::StateKeyMap() {
    std::ifstream  inputFile(ENGINE_PATH "/res/inputs/keyboard_shortcuts.json");
    nlohmann::json loadedKeyboardShortcuts = nlohmann::json::parse(inputFile);
    for (nlohmann::json::iterator it = loadedKeyboardShortcuts.begin();
         it != loadedKeyboardShortcuts.end(); it++) {
      unsigned int stateKey = std::stoi(it.key());
      StateType    type = static_cast<StateType>(stateKey);
      for (nlohmann::json::iterator jt = it->begin(); jt != it->end(); jt++) {
        std::vector<std::vector<unsigned int>> keyCombination(jt.value());
        unsigned int shortcutKey = std::stoi(jt.key());
        this->stateKeyMap[type][shortcutKey] = keyCombination;
      }
    }
  }

  std::unordered_map<unsigned int, std::vector<std::vector<unsigned int>>> &
      StateKeyMap::getInputMap(StateType type) {
    return this->stateKeyMap[type];
  }
}