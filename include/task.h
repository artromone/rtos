// task.h
#ifndef TASK_H
#define TASK_H

#include <functional>
#include <vector>

namespace RTOS {

class Event;

class Task {
private:
  int id;
  int priority;
  int period; // Для RMA
  bool ready;
  std::function<void()> taskFunction;
  std::vector<Event *> ownedEvents;

public:
  Task(int id, int priority, int period, std::function<void()> func);

  int getId() const;
  int getPriority() const;
  void setPriority(int newPriority);
  int getPeriod() const;
  bool isReady() const;
  void setReady(bool state);

  void execute();
  void addEvent(Event *event);
  std::vector<Event *> &getEvents();
};

} // namespace RTOS

#endif // TASK_H
