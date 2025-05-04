// semaphore.h
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "system_log.h"
#include "task.h"
#include <condition_variable>
#include <mutex>
#include <vector>

namespace RTOS {

class Semaphore {
private:
  std::mutex mtx;
  std::condition_variable cv;
  int count;
  int originalOwnerPriority;
  Task *owner;
  std::vector<Task *> waitingTasks;
  SystemLog &logger;

public:
  Semaphore(int initialCount = 1);

  const std::vector<Task *> &getWaitingTasks() const { return waitingTasks; }

  bool acquire(Task *task);
  void release(Task *task);
  Task *getOwner() const;
};

} // namespace RTOS

#endif // SEMAPHORE_H
