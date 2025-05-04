// event.h
#ifndef EVENT_H
#define EVENT_H

#include "system_log.h"
#include "task.h"
#include <vector>

namespace RTOS {

class Event {
private:
  int id;
  Task *owner;
  bool triggered;
  std::vector<Task *> waitingTasks;
  SystemLog &logger;

public:
  Event(int id, Task *owner);

  int getId() const;
  Task *getOwner() const;

  void trigger();
  void reset();
  bool isTriggered() const;
  void waitFor(Task *task);
};

} // namespace RTOS

#endif // EVENT_H
