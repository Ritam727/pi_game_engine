#include "event_manager.hpp"
#include <utility>

namespace events {
  EventManager::EventManager() {}

  void EventManager::executeEvents() {
    for (std::pair<const std::type_index,
                   std::array<std::vector<std::unique_ptr<BaseEvent>>, 2>> &p :
         this->topics) {
      {
        std::lock_guard<std::mutex> lock = std::lock_guard(this->swapMutex);
        this->read = (this->read + 1) % 2;
        this->write = (this->write + 1) % 2;
      }
      const std::type_index                    idx = p.first;
      std::vector<std::unique_ptr<BaseEvent>> &events = p.second[read];
      if (this->subscribers.contains(idx)) {
        logger::info("Processing {} events", events.size());
        std::vector<std::unique_ptr<EventHandle>> &handles =
            this->subscribers[idx];
        for (int i = 0; i < events.size(); i--) {
          for (std::unique_ptr<EventHandle> &handle : handles)
            (handle.get())->execute(events[i]);
        }
      }
      events.clear();
    }
  }

  EventManager &EventManager::getInstance() {
    static EventManager instance;
    return instance;
  }
}