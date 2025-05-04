// test_pip.cpp
#include "../include/rtos.h"
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

void testPriorityInheritanceProtocol() {
  RTOS::Scheduler scheduler;
  RTOS::SystemLog &logger = RTOS::SystemLog::getInstance();
  logger.clearLog();

  // Флаги для отслеживания выполнения задач
  bool lowPriorityTaskStarted = false;
  bool lowPriorityTaskHoldingSemaphore = false;
  bool mediumPriorityTaskStarted = false;
  bool mediumPriorityTaskCompleted = false;
  bool highPriorityTaskStarted = false;
  bool highPriorityTaskCompleted = false;

  // Создаем семафор для тестирования PIP
  auto semaphore = scheduler.createSemaphore();

  RTOS::Task *lowPriorityTask = nullptr;
  RTOS::Task *mediumPriorityTask = nullptr;
  RTOS::Task *highPriorityTask = nullptr;

  // Задача с низким приоритетом, которая захватывает семафор
  lowPriorityTask = scheduler.createTask(0, 300, [&]() {
    lowPriorityTaskStarted = true;

    // Захват семафора
    semaphore->acquire(lowPriorityTask);
    lowPriorityTaskHoldingSemaphore = true;

    // Имитация длительной работы с семафором
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Освобождение семафора
    semaphore->release(lowPriorityTask);

    std::cout << "Low priority task completed" << std::endl;
  });

  // Задача со средним приоритетом, которая пытается вытеснить задачу с низким
  // приоритетом
  mediumPriorityTask = scheduler.createTask(0, 200, [&]() {
    mediumPriorityTaskStarted = true;

    // Имитация работы
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    mediumPriorityTaskCompleted = true;
    std::cout << "Medium priority task completed" << std::endl;
  });

  // Задача с высоким приоритетом, которая пытается получить семафор
  highPriorityTask = scheduler.createTask(0, 100, [&]() {
    highPriorityTaskStarted = true;

    // Пытаемся захватить семафор, который уже захвачен задачей с низким
    // приоритетом
    semaphore->acquire(highPriorityTask);

    // Если PIP работает правильно, мы должны получить семафор после того,
    // как задача с низким приоритетом его освободит
    assert(lowPriorityTaskHoldingSemaphore);

    // Имитация работы с семафором
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Освобождаем семафор
    semaphore->release(highPriorityTask);

    highPriorityTaskCompleted = true;
    std::cout << "High priority task completed" << std::endl;
  });

  // Запускаем планировщик
  scheduler.start();

  // Даем задаче с низким приоритетом захватить семафор
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  assert(lowPriorityTaskStarted);
  assert(lowPriorityTaskHoldingSemaphore);

  // Активируем задачу с высоким приоритетом, которая будет блокирована на
  // семафоре
  highPriorityTask->setReady(true);
  std::this_thread::sleep_for(
      std::chrono::milliseconds(20)); // Увеличиваем время ожидания

  // Проверяем, что задача с низким приоритетом унаследовала приоритет задачи с
  // высоким приоритетом
  assert(lowPriorityTask->getPriority() == highPriorityTask->getPriority());

  // Активируем задачу со средним приоритетом
  mediumPriorityTask->setReady(true);

  // Ждем завершения всех задач
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  scheduler.stop();

  // Проверяем, что все задачи завершились
  assert(highPriorityTaskCompleted);
  assert(mediumPriorityTaskCompleted);

  // Проверяем логи на наличие записей о наследовании приоритета
  const auto &logs = logger.getLog();
  bool foundPriorityInheritanceLog = false;

  for (const auto &log : logs) {
    if (log.find("inherited priority") != std::string::npos) {
      foundPriorityInheritanceLog = true;
      break;
    }
  }

  assert(foundPriorityInheritanceLog);

  std::cout << "Priority Inheritance Protocol test: PASSED" << std::endl;
}
