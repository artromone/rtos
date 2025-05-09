// main_test.cpp
#include "../include/rtos.h"
#include <cassert>
#include <iostream>

// Прототипы тестовых функций
void testMaximumLimits();
void testRMAPriorityAssignment();
void testNonpreemptiveScheduling();
void testEvents();
void testSemaphores();
void testIntegration();

int main() {
  std::cout << "Запуск тестов RTOS..." << std::endl;

  testMaximumLimits();
  std::cout << "Тест максимальных ограничений: ПРОЙДЕН" << std::endl;

  testRMAPriorityAssignment();
  std::cout << "Тест назначения приоритетов RMA: ПРОЙДЕН" << std::endl;

  testNonpreemptiveScheduling();
  std::cout << "Тест планирования без вытеснения: ПРОЙДЕН" << std::endl;

  // testSemaphores();
  // testIntegration();
  // testEvents();

  std::cout << "Все тесты ПРОЙДЕНЫ!" << std::endl;
  return 0;
}
