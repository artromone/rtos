// test_rma.cpp
#include "../include/rtos.h"
#include <cassert>
#include <chrono>
#include <thread>

void testRMAPriorityAssignment() {
  RTOS::Scheduler scheduler;

  auto task1 = scheduler.createTask(0, 100, []() {}); // Период 100
  auto task2 = scheduler.createTask(0, 50, []() {});  // Период 50
  auto task3 = scheduler.createTask(0, 200, []() {}); // Период 200

  scheduler.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  scheduler.stop();

  // По RMA, задачи с меньшим периодом должны иметь более высокий приоритет
  assert(task2->getPriority() > task1->getPriority());
  assert(task1->getPriority() > task3->getPriority());
}
