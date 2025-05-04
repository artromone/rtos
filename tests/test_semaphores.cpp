// test_semaphores.cpp
#include "../include/rtos.h"
#include <cassert>
#include <chrono>
#include <thread>

void testSemaphores() {
  RTOS::Scheduler scheduler;
  RTOS::SystemLog &logger = RTOS::SystemLog::getInstance();
  logger.clearLog();

  bool task1Executed = false;
  bool task2Executed = false;
  bool task3Executed = false;

  // Создаем семафор
  auto semaphore = scheduler.createSemaphore();
  assert(semaphore != nullptr);

  RTOS::Task *task1 = nullptr;
  RTOS::Task *task2 = nullptr;
  RTOS::Task *task3 = nullptr;

  // Задача с низким приоритетом, которая захватывает семафор
  task1 = scheduler.createTask(0, 300, [&]() {
    assert(semaphore->acquire(task1)); // Должен успешно захватить семафор
    std::this_thread::sleep_for(
        std::chrono::milliseconds(50)); // Имитация работы
    task1Executed = true;
    semaphore->release(task1);
  });

  // Задача со средним приоритетом, которая просто выполняется
  task2 = scheduler.createTask(0, 200, [&]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    task2Executed = true;
  });

  // Задача с высоким приоритетом, которая пытается захватить семафор
  task3 = scheduler.createTask(0, 100, [&]() {
    assert(!semaphore->acquire(
        task3)); // Не должен получить семафор, т.к. он занят
    task3Executed = true;
  });

  // Запускаем планировщик
  scheduler.start();

  // Ждем достаточное время для выполнения всех задач
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  scheduler.stop();

  // Проверяем, что все задачи выполнились
  assert(task1Executed);
  assert(task2Executed);
  assert(task3Executed);

  // Проверяем, что приоритет task1 был временно повышен (наследование
  // приоритета)
  auto logs = logger.getLog();
  bool priorityInheritanceFound = false;
  for (const auto &log : logs) {
    if (log.find("inherited priority") != std::string::npos) {
      priorityInheritanceFound = true;
      break;
    }
  }
  assert(priorityInheritanceFound);
}
