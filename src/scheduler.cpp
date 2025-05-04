// scheduler.cpp
#include "../include/scheduler.h"
#include "../include/rtos.h"
#include <algorithm>

namespace RTOS {

Scheduler::Scheduler() : logger(SystemLog::getInstance()), running(false) {
  tasks.reserve(MAX_TASKS);
  semaphores.reserve(MAX_RESOURCES);
  events.reserve(MAX_EVENTS);
}

Scheduler::~Scheduler() {
  stop();
  for (auto task : tasks)
    delete task;
  for (auto semaphore : semaphores)
    delete semaphore;
  for (auto event : events)
    delete event;
}

Task *Scheduler::createTask(int priority, int period,
                            std::function<void()> taskFunction) {
  if (tasks.size() >= MAX_TASKS) {
    logger.logEvent("ERROR: Maximum number of tasks reached");
    return nullptr;
  }

  if (priority >= MAX_PRIORITIES) {
    logger.logEvent("ERROR: Priority exceeds maximum allowed");
    return nullptr;
  }

  int id = static_cast<int>(tasks.size());
  Task *task = new Task(id, priority, period, taskFunction);
  tasks.push_back(task);

  logger.logEvent("Task " + std::to_string(id) + " created with priority " +
                  std::to_string(priority) + " and period " +
                  std::to_string(period));

  return task;
}

Semaphore *Scheduler::createSemaphore() {
  if (semaphores.size() >= MAX_RESOURCES) {
    logger.logEvent("ERROR: Maximum number of semaphores reached");
    return nullptr;
  }

  Semaphore *semaphore = new Semaphore();
  semaphores.push_back(semaphore);

  logger.logEvent("Semaphore " + std::to_string(semaphores.size() - 1) +
                  " created");

  return semaphore;
}

Event *Scheduler::createEvent(Task *owner) {
  if (events.size() >= MAX_EVENTS) {
    logger.logEvent("ERROR: Maximum number of events reached");
    return nullptr;
  }

  int id = static_cast<int>(events.size());
  Event *event = new Event(id, owner);
  events.push_back(event);

  logger.logEvent(
      "Event " + std::to_string(id) + " created" +
      (owner ? " owned by Task " + std::to_string(owner->getId()) : ""));

  return event;
}

void Scheduler::start() {
  if (running)
    return;

  running = true;
  logger.logEvent("Scheduler started");

  // Сортировка задач согласно RMA (меньший период = выше приоритет)
  std::sort(tasks.begin(), tasks.end(),
            [](Task *a, Task *b) { return a->getPeriod() < b->getPeriod(); });

  // Обновление приоритетов на основе RMA
  for (size_t i = 0; i < tasks.size() && i < MAX_PRIORITIES; ++i) {
    int rmaPriority = MAX_PRIORITIES - 1 - static_cast<int>(i);
    tasks[i]->setPriority(rmaPriority);
    logger.logEvent("Task " + std::to_string(tasks[i]->getId()) +
                    " RMA priority set to " + std::to_string(rmaPriority));
  }

  // Запуск планировщика в отдельном потоке
  schedulerThread = std::thread([this]() { this->schedulerLoop(); });
}

void Scheduler::schedulerLoop() {
  while (running) {
    // Поиск готовой задачи с наивысшим приоритетом (без вытеснения)
    Task *selectedTask = nullptr;

    for (auto task : tasks) {
      if (task->isReady()) {
        if (!selectedTask ||
            task->getPriority() > selectedTask->getPriority()) {
          selectedTask = task;
        }
      }
    }

    if (selectedTask) {
      logger.logEvent("Task " + std::to_string(selectedTask->getId()) +
                      " selected for execution");
      selectedTask->execute();
      logger.logEvent("Task " + std::to_string(selectedTask->getId()) +
                      " completed execution");
    } else {
      // Нет готовых задач
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
}

void Scheduler::stop() {
  if (!running)
    return;

  running = false;
  logger.logEvent("Scheduler stopping");

  if (schedulerThread.joinable()) {
    schedulerThread.join();
  }

  logger.logEvent("Scheduler stopped");
}

const std::vector<Task *> &Scheduler::getTasks() const { return tasks; }

const std::vector<Semaphore *> &Scheduler::getSemaphores() const {
  return semaphores;
}

const std::vector<Event *> &Scheduler::getEvents() const { return events; }

bool Scheduler::isRunning() const { return running; }

} // namespace RTOS
