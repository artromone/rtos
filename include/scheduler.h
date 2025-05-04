// scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "event.h"
#include "semaphore.h"
#include "system_log.h"
#include "task.h"
#include <functional>
#include <thread>
#include <vector>

namespace RTOS {

class Scheduler {
private:
  std::vector<Task *> tasks;
  std::vector<Semaphore *> semaphores;
  std::vector<Event *> events;
  SystemLog &logger;
  bool running;
  std::thread schedulerThread;

  void schedulerLoop();

public:
  Scheduler();
  ~Scheduler();

  static Scheduler &getInstance() {
    static Scheduler instance;
    return instance;
  }

  Task *createTask(int priority, int period,
                   std::function<void()> taskFunction);
  Semaphore *createSemaphore();
  Event *createEvent(Task *owner);

  void start();
  void stop();

  const std::vector<Task *> &getTasks() const;
  const std::vector<Semaphore *> &getSemaphores() const;
  const std::vector<Event *> &getEvents() const;
  bool isRunning() const;
};

} // namespace RTOS

#endif // SCHEDULER_H
