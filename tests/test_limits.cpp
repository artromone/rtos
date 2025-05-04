// test_limits.cpp
#include "../include/rtos.h"
#include <cassert>

void testMaximumLimits() {
  RTOS::Scheduler scheduler;

  // Тест максимального количества задач (32)
  for (int i = 0; i < RTOS::MAX_TASKS; ++i) {
    auto task = scheduler.createTask(0, 100, []() {});
    assert(task != nullptr);
  }
  auto extraTask = scheduler.createTask(0, 100, []() {});
  assert(extraTask == nullptr);

  // Тест максимального количества семафоров (16)
  for (int i = 0; i < RTOS::MAX_RESOURCES; ++i) {
    auto semaphore = scheduler.createSemaphore();
    assert(semaphore != nullptr);
  }
  auto extraSemaphore = scheduler.createSemaphore();
  assert(extraSemaphore == nullptr);

  // Тест максимального количества событий (16)
  auto task = scheduler.getTasks()[0];
  for (int i = 0; i < RTOS::MAX_EVENTS; ++i) {
    auto event = scheduler.createEvent(task);
    assert(event != nullptr);
  }
  auto extraEvent = scheduler.createEvent(task);
  assert(extraEvent == nullptr);
}
