// test_events.cpp
#include "../include/rtos.h"
#include <cassert>
#include <chrono>
#include <thread>

void testEventManagement() {
  RTOS::Scheduler scheduler;

  bool task2Executed = false;

  auto task1 = scheduler.createTask(1, 100, []() {});
  auto task2 = scheduler.createTask(0, 200, [&]() { task2Executed = true; });

  auto event = scheduler.createEvent(task1);

  // Task2 ожидает событие
  event->waitFor(task2);
  assert(!task2->isReady());

  // Task1 активирует событие
  event->trigger();
  assert(task2->isReady());

  scheduler.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  scheduler.stop();

  assert(task2Executed);
}
