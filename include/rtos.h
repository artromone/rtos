// rtos.h
#ifndef RTOS_H
#define RTOS_H

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "event.h"
#include "scheduler.h"
#include "semaphore.h"
#include "system_log.h"
#include "task.h"

namespace RTOS {

// Константы системы
constexpr int MAX_TASKS = 32;
constexpr int MAX_PRIORITIES = 16;
constexpr int MAX_RESOURCES = 16;
constexpr int MAX_EVENTS = 16;

} // namespace RTOS

#endif // RTOS_H
