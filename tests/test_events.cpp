// test_events.cpp
#include "../include/rtos.h"
#include <cassert>
#include <chrono>
#include <thread>

void testEvents() {
  RTOS::Scheduler scheduler;
  RTOS::SystemLog &logger = RTOS::SystemLog::getInstance();
  logger.clearLog();

  bool ownerTaskExecuted = false;
  bool waitingTask1Executed = false;
  bool waitingTask2Executed = false;

  RTOS::Task *ownerTask = nullptr;

  // Создаем задачу-владельца события
  ownerTask = scheduler.createTask(0, 100, [&]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    ownerTaskExecuted = true;

    // Активируем событие
    auto events = ownerTask->getEvents();
    assert(!events.empty());
    events[0]->trigger();
  });

  // Создаем событие, принадлежащее ownerTask
  auto event = scheduler.createEvent(ownerTask);
  assert(event != nullptr);
  assert(event->getOwner() == ownerTask);

  // Создаем задачи, ожидающие событие
  auto waitingTask1 =
      scheduler.createTask(0, 200, [&]() { waitingTask1Executed = true; });

  auto waitingTask2 =
      scheduler.createTask(0, 300, [&]() { waitingTask2Executed = true; });

  // Задачи ожидают событие
  event->waitFor(waitingTask1);
  event->waitFor(waitingTask2);

  // Проверяем, что задачи перешли в состояние ожидания
  assert(!waitingTask1->isReady());
  assert(!waitingTask2->isReady());

  // Запускаем планировщик
  scheduler.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  scheduler.stop();

  // Проверяем, что все задачи выполнились
  assert(ownerTaskExecuted);
  assert(waitingTask1Executed);
  assert(waitingTask2Executed);

  // Проверяем, что событие было активировано
  assert(event->isTriggered());
}
