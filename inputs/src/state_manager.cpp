#include "state_manager.hpp"

#include "inputs_constants.hpp"
#include "nlohmann/json.hpp"
#include "states.hpp"
#include <fstream>
#include <string>

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace inputs {
  StateManager::StateManager() {
    std::ifstream  inputFile(ENGINE_PATH "/res/inputs/keyboard_shortcuts.json");
    nlohmann::json loadedKeyboardShortcuts = nlohmann::json::parse(inputFile);
    for (nlohmann::json::iterator it = loadedKeyboardShortcuts.begin();
         it != loadedKeyboardShortcuts.end(); it++) {
      std::string keyPress = it.key();
      for (nlohmann::json::iterator jt = it->begin(); jt != it->end(); jt++) {
        std::array<std::string, 2> keyCombination(jt.value());
        std::string                type = jt.key();
        if (type == "inputs::CameraMoveMode") {
          this->activations.insert(
              {keyPress,
               StateManager::createActivation<CameraMoveMode>(keyCombination)});
        } else if (type == "inputs::CameraViewMode") {
          this->activations.insert(
              {keyPress,
               StateManager::createActivation<CameraViewMode>(keyCombination)});
        }
      }
    }
  }

  StateManager &StateManager::getInstance() {
    static StateManager stateManager;
    return stateManager;
  }

  void StateManager::addKey(unsigned int key) {
    std::vector<unsigned int> &buttonsPressed = this->inputState.buttonsPressed;
    std::string currentActivation = this->getFirstMatch(0, std::string{});
    for (unsigned int &pressedKey : buttonsPressed) {
      if (pressedKey == key) {
        return;
      }
    }
    if (buttonsPressed.size() == Constants::KEYBOARD_SHORTCUT_DEPTH)
      return;
    buttonsPressed.emplace_back(key);
    if (currentActivation.empty()) {
      std::string firstMatch = this->getFirstMatch(0, std::string{});
      if (!firstMatch.empty()) {
        this->activations[firstMatch].activated = true;
        this->activations[firstMatch].onPress(this->modeManager);
      }
    }
  }

  void StateManager::removeKey(unsigned int key) {
    std::vector<unsigned int> &buttonsPressed = this->inputState.buttonsPressed;
    std::string currentActivation = this->getFirstMatch(0, std::string{});
    if (this->currentActivationContainsKey(key) &&
        this->activations[currentActivation].activated) {
      this->activations[currentActivation].activated = false;
      this->activations[currentActivation].onRelease(this->modeManager);
    }
    unsigned int idx = buttonsPressed.size();
    for (unsigned int i = 0; i < buttonsPressed.size(); i++) {
      if (buttonsPressed[i] == key) {
        idx = i;
      }
    }
    for (unsigned int i = idx; i < buttonsPressed.size() - 1; i++)
      buttonsPressed[i] = buttonsPressed[i + 1];
    buttonsPressed.pop_back();
  }

  bool StateManager::currentActivationContainsKey(unsigned int key) {
    std::vector<std::string> activationKeys =
        this->getKeysInCurrentActivation();
    for (std::string &activationKey : activationKeys) {
      if (this->keyToStringMap[key] == activationKey) {
        return true;
      }
    }
    return false;
  }

  std::vector<std::string> StateManager::getKeysInCurrentActivation() {
    std::string              firstMatch = this->getFirstMatch(0, std::string{});
    std::vector<std::string> keys;
    std::string              current{};
    for (char c : firstMatch) {
      if (c == Constants::UNDERSCORE) {
        keys.push_back(current);
        current.clear();
      } else {
        current += c;
      }
    }
    if (!current.empty()) {
      keys.push_back(current);
    }
    return keys;
  }

  std::string StateManager::getFirstMatch(unsigned int idx,
                                          std::string  current) {
    std::vector<unsigned int> buttonsPressed = this->inputState.buttonsPressed;
    if (idx == buttonsPressed.size()) {
      return this->activations.contains(current) ? current
                                                 : Constants::EMPTY_STRING;
    }
    std::string taken = current + Constants::UNDERSCORE +
                        this->keyToStringMap[buttonsPressed[idx]];
    std::string next = this->getFirstMatch(idx + 1, taken);
    return next.size() > 0 ? next : this->getFirstMatch(idx + 1, current);
  }
}