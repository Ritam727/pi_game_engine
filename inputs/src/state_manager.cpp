#include "state_manager.hpp"

#include "GLFW/glfw3.h"
#include "nlohmann/json.hpp"
#include "states.hpp"
#include <fstream>
#include <iostream>
#include <string>

#ifndef ENGINE_PATH
#define ENGINE_PATH "/"
#endif

namespace inputs {
  StateManager::StateManager() : state({CameraState::DEFAULT}) {
    nlohmann::json keyboardShortcuts = nlohmann::json::object();
    std::string    st =
        std::to_string(static_cast<unsigned int>(StateType::CAMERA_STATE));
    keyboardShortcuts[st] = nlohmann::json::object();
    for (CameraState &cameraState : enumValues(CameraState)) {
      std::string val = std::to_string(static_cast<unsigned int>(cameraState));
      if (cameraState == CameraState::CAMERA_PAN_MODE) {
        keyboardShortcuts[st][val] =
            this->inputStateMap[StateType::CAMERA_STATE]
                               [static_cast<unsigned int>(
                                   CameraState::CAMERA_PAN_MODE)] = {
                {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL},
                {GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT}};
      } else if (cameraState == CameraState::CAMERA_FLY_MODE) {
        keyboardShortcuts[st][val] =
            this->inputStateMap[StateType::CAMERA_STATE]
                               [static_cast<unsigned int>(
                                   CameraState::CAMERA_FLY_MODE)] = {
                {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL},
                {GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT}};
      } else if (cameraState == CameraState::CAMERA_RESET_MODE) {
        keyboardShortcuts[st][val] =
            this->inputStateMap[StateType::CAMERA_STATE]
                               [static_cast<unsigned int>(
                                   CameraState::CAMERA_RESET_MODE)] = {
                {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL},
                {GLFW_KEY_C},
                {GLFW_KEY_R}};
      }
    }
    std::cout << std::setw(2) << keyboardShortcuts;
    std::ofstream outputFile(ENGINE_PATH "/res/inputs/keyboard_shortcuts.json");
    outputFile << std::setw(2) << keyboardShortcuts;
  }

  State &StateManager::getState() {
    return this->state;
  }

  std::unordered_map<unsigned int, std::vector<std::vector<unsigned int>>> &
      StateManager::getInputMap(StateType type) {
    return this->inputStateMap[type];
  }
}