// event.cpp
#include "../include/event.h"

namespace RTOS {

Event::Event(int id, Task *owner)
    : id(id), owner(owner), triggered(false), logger(SystemLog::getInstance()) {
  if (owner) {
    owner->addEvent(this);
  }
}

int Event::getId() const { return id; }

Task *Event::getOwner() const { return owner; }

void Event::trigger() {
  if (owner) {
    triggered = true;
    logger.logEvent("Event " + std::to_string(id) + " triggered by Task " +
                    std::to_string(owner->getId()));

    for (auto task : waitingTasks) {
      task->setReady(true);
      logger.logEvent("Task " + std::to_string(task->getId()) +
                      " woken up by event " + std::to_string(id));
    }
    waitingTasks.clear();
  }
}

void Event::reset() {
  triggered = false;
  logger.logEvent("Event " + std::to_string(id) + " reset");
}

bool Event::isTriggered() const { return triggered; }

void Event::waitFor(Task *task) {
  if (!triggered && task != owner) {
    waitingTasks.push_back(task);
    task->setReady(false);
    logger.logEvent("Task " + std::to_string(task->getId()) +
                    " waiting for event " + std::to_string(id));
  }
}

} // namespace RTOS
