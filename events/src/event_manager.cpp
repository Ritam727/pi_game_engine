#include "event_manager.hpp"
#include <utility>

namespace events {
  EventManager::EventManager() {}

  void EventManager::executeEvents() {
    for (std::pair<const std::type_index,
                   std::vector<std::unique_ptr<BaseEvent>>> &p : this->topics) {
      const std::type_index                    idx = p.first;
      std::vector<std::unique_ptr<BaseEvent>> &events = p.second;
      if (this->subscribers.contains(idx)) {
        std::vector<std::unique_ptr<EventHandle>> &handles =
            this->subscribers[idx];
        int eventCount = events.size();
        for (int i = eventCount - 1; i >= 0; i--) {
          for (std::unique_ptr<EventHandle> &handle : handles) {
            (handle.get())->execute(events[i]);
          }
          events.pop_back();
        }
      }
    }
  }

  EventManager &EventManager::getInstance() {
    static EventManager instance;
    return instance;
  }
}