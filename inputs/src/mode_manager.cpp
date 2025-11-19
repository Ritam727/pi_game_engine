#include "mode_manager.hpp"
#include "modes.hpp"

namespace inputs {
  std::unique_ptr<IMode> &ModeManager::getMode(std::string name) {
    return this->modeMap[name];
  }
}