// system_log.cpp
#include "../include/system_log.h"

namespace RTOS {

void SystemLog::logEvent(const std::string &eventDescription) {
  std::lock_guard<std::mutex> lock(logMutex);
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);

  std::string timeStr = std::ctime(&time);
  if (!timeStr.empty() && timeStr.back() == '\n') {
    timeStr.pop_back();
  }

  eventLog.push_back("[" + timeStr + "] " + eventDescription);
}

const std::vector<std::string> &SystemLog::getLog() const { return eventLog; }

void SystemLog::clearLog() {
  std::lock_guard<std::mutex> lock(logMutex);
  eventLog.clear();
}

} // namespace RTOS
