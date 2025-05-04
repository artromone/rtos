// task.cpp
#include "../include/task.h"
#include "../include/event.h"

namespace RTOS {

Task::Task(int id, int priority, int period, std::function<void()> func)
    : id(id), priority(priority), period(period), ready(true),
      taskFunction(func) {}

int Task::getId() const { return id; }

int Task::getPriority() const { return priority; }

void Task::setPriority(int newPriority) { priority = newPriority; }

int Task::getPeriod() const { return period; }

bool Task::isReady() const { return ready; }

void Task::setReady(bool state) { ready = state; }

void Task::execute() {
  if (taskFunction) {
    taskFunction();
  }
}

void Task::addEvent(Event *event) { ownedEvents.push_back(event); }

std::vector<Event *> &Task::getEvents() { return ownedEvents; }

} // namespace RTOS
