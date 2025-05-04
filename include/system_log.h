// system_log.h
#ifndef SYSTEM_LOG_H
#define SYSTEM_LOG_H

#include <chrono>
#include <ctime>
#include <mutex>
#include <string>
#include <vector>

namespace RTOS {

class SystemLog {
private:
  std::vector<std::string> eventLog;
  std::mutex logMutex;

  SystemLog() = default;
  SystemLog(const SystemLog &) = delete;
  SystemLog &operator=(const SystemLog &) = delete;

public:
  static SystemLog &getInstance() {
    static SystemLog instance;
    return instance;
  }

  void logEvent(const std::string &eventDescription);
  const std::vector<std::string> &getLog() const;
  void clearLog();
};

} // namespace RTOS

#endif // SYSTEM_LOG_H
