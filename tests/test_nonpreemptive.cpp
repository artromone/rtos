// test_nonpreemptive.cpp
#include "../include/rtos.h"
#include <cassert>
#include <chrono>
#include <thread>

void testNonpreemptiveScheduling() {
  RTOS::Scheduler scheduler;
  RTOS::SystemLog &logger = RTOS::SystemLog::getInstance();
  logger.clearLog();

  bool task1Completed = false;
  bool task2Started = false;

  // Создаем task1 с большим периодом (низкий приоритет по RMA)
  auto task1 = scheduler.createTask(0, 200, [&]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    task1Completed = true;
  });

  // Создаем task2 с меньшим периодом (высокий приоритет по RMA)
  auto task2 = scheduler.createTask(0, 100, [&]() { task2Started = true; });

  // Устанавливаем task1 в готовое состояние, а task2 - в неготовое
  task1->setReady(true);
  task2->setReady(false);

  scheduler.start();

  // Даем task1 начать выполнение
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // Теперь делаем task2 готовой к выполнению
  task2->setReady(true);

  // Ждем достаточное время для завершения обеих задач
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  scheduler.stop();

  // При планировании без вытеснения task1 должен завершиться до начала task2,
  // даже если task2 имеет более высокий приоритет
  assert(task1Completed);
  assert(task2Started);
}
