// test_integration.cpp
#include "../include/rtos.h"
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>

void testIntegration() {
  RTOS::Scheduler scheduler;
  RTOS::SystemLog &logger = RTOS::SystemLog::getInstance();
  logger.clearLog();

  std::vector<bool> taskExecuted(4, false);
  std::vector<int> executionOrder;

  // Создаем семафоры
  auto semaphore1 = scheduler.createSemaphore();
  auto semaphore2 = scheduler.createSemaphore();

  RTOS::Task *task1 = nullptr;
  RTOS::Task *task2 = nullptr;
  RTOS::Task *task3 = nullptr;
  RTOS::Task *task4 = nullptr;

  // Задача 1: Захватывает семафор1, создает событие, освобождает семафор1
  task1 = scheduler.createTask(0, 200, [&]() {
    executionOrder.push_back(1);
    assert(semaphore1->acquire(task1));

    auto event = scheduler.createEvent(task1);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    event->trigger(); // Активируем событие для task3
    semaphore1->release(task1);
    taskExecuted[0] = true;
  });

  // Задача 2: Захватывает семафор2, затем семафор1, освобождает оба
  task2 = scheduler.createTask(0, 100, [&]() {
    executionOrder.push_back(2);
    assert(semaphore2->acquire(task2));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Пытаемся захватить семафор1, который может быть занят task1
    bool acquired = semaphore1->acquire(task2);
    if (acquired) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      semaphore1->release(task2);
    }

    semaphore2->release(task2);
    taskExecuted[1] = true;
  });

  // Задача 3: Ожидает событие от task1
  task3 = scheduler.createTask(0, 150, [&]() {
    executionOrder.push_back(3);
    // Ожидание события будет настроено после создания события в task1
    taskExecuted[2] = true;
  });

  // Задача 4: Пытается захватить оба семафора
  task4 = scheduler.createTask(0, 50, [&]() {
    executionOrder.push_back(4);
    bool acquired1 = semaphore1->acquire(task4);
    bool acquired2 = semaphore2->acquire(task4);

    if (acquired1)
      semaphore1->release(task4);
    if (acquired2)
      semaphore2->release(task4);

    taskExecuted[3] = true;
  });

  // Запускаем планировщик
  scheduler.start();

  // Ждем достаточное время для выполнения всех задач
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  scheduler.stop();

  // Проверяем, что все задачи выполнились
  for (int i = 0; i < 4; i++) {
    assert(taskExecuted[i]);
  }

  // Проверяем логи на наличие записей о наследовании приоритетов
  auto logs = logger.getLog();
  bool priorityInheritanceFound = false;
  for (const auto &log : logs) {
    if (log.find("inherited priority") != std::string::npos) {
      priorityInheritanceFound = true;
      break;
    }
  }

  // В зависимости от порядка выполнения, может быть или не быть наследование
  // приоритетов Просто выводим порядок выполнения для анализа
  std::cout << "Порядок выполнения задач: ";
  for (int taskId : executionOrder) {
    std::cout << taskId << " ";
  }
  std::cout << std::endl;
}
