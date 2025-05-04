// semaphore.cpp
#include "../include/semaphore.h"
#include "../include/scheduler.h"
#include <algorithm>

namespace RTOS {

Semaphore::Semaphore(int initialCount)
    : count(initialCount), owner(nullptr), originalOwnerPriority(-1),
      logger(SystemLog::getInstance()) {}

bool Semaphore::acquire(Task *task) {
  std::unique_lock<std::mutex> lock(mtx);

  if (count > 0) {
    // Ресурс доступен
    count--;
    owner = task;
    originalOwnerPriority = task->getPriority();
    logger.logEvent("Task " + std::to_string(task->getId()) +
                    " acquired semaphore");
    return true;
  } else {
    // Ресурс недоступен
    waitingTasks.push_back(task);

    // Реализация Priority Inheritance - исправленная версия
    if (owner) {
      int oldPriority = owner->getPriority();
      if (task->getPriority() > owner->getPriority()) {
        owner->setPriority(task->getPriority());
        logger.logEvent("Task " + std::to_string(owner->getId()) +
                        " inherited priority " +
                        std::to_string(task->getPriority()) + " from Task " +
                        std::to_string(task->getId()) + " (was " +
                        std::to_string(oldPriority) + ")");
      }
    }

    task->setReady(false);
    logger.logEvent("Task " + std::to_string(task->getId()) +
                    " waiting for semaphore");
    return false;
  }
}

void Semaphore::release(Task *task) {
  std::unique_lock<std::mutex> lock(mtx);

  if (owner == task) {
    // Перед восстановлением приоритета нужно проверить другие семафоры
    if (originalOwnerPriority != -1) {
      // Проверяем, удерживает ли задача другие семафоры с наследованием
      // приоритета
      bool canRestorePriority = true;
      int highestWaiterPriority = originalOwnerPriority;

      // Получаем список всех семафоров из планировщика
      auto &allSemaphores = Scheduler::getInstance().getSemaphores();

      // Проверяем каждый семафор, который может удерживать эта задача
      for (auto sem : allSemaphores) {
        if (sem != this && sem->getOwner() == task) {
          // Находим задачу с наивысшим приоритетом среди ожидающих этот семафор
          for (auto &waiter : sem->getWaitingTasks()) {
            if (waiter->getPriority() > highestWaiterPriority) {
              highestWaiterPriority = waiter->getPriority();
              canRestorePriority = false;
            }
          }
        }
      }

      // Восстанавливаем приоритет только если нет других семафоров с ожидающими
      // задачами высокого приоритета
      if (canRestorePriority) {
        logger.logEvent("Task " + std::to_string(task->getId()) +
                        " restored to original priority " +
                        std::to_string(originalOwnerPriority));
        task->setPriority(originalOwnerPriority);
      } else {
        logger.logEvent("Task " + std::to_string(task->getId()) +
                        " maintains inherited priority " +
                        std::to_string(highestWaiterPriority) +
                        " due to other semaphores");
        task->setPriority(highestWaiterPriority);
      }

      originalOwnerPriority = -1;
    }

    owner = nullptr;
    count++;

    // Поиск задачи с наивысшим приоритетом среди ожидающих
    if (!waitingTasks.empty()) {
      auto highestPriorityTask = *std::max_element(
          waitingTasks.begin(), waitingTasks.end(),
          [](Task *a, Task *b) { return a->getPriority() < b->getPriority(); });

      waitingTasks.erase(std::remove(waitingTasks.begin(), waitingTasks.end(),
                                     highestPriorityTask),
                         waitingTasks.end());

      highestPriorityTask->setReady(true);
      logger.logEvent("Task " + std::to_string(highestPriorityTask->getId()) +
                      " woken up after semaphore release");
    }

    logger.logEvent("Task " + std::to_string(task->getId()) +
                    " released semaphore");
  }
}

Task *Semaphore::getOwner() const { return owner; }

} // namespace RTOS
