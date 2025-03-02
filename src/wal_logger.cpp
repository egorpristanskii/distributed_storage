#include "wal_logger.h"

#include "utils.h"

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace storage {
WALLogger::WALLogger(const std::string& logFile)
    : logFile_(std::move(logFile)) {
    std::ofstream file(logFile_, std::ios::app);
}

void WALLogger::logOperation(const std::string& operation,
                             const std::string& key, const std::string& value) {
    std::unique_lock<std::mutex> lock_guard(mtx_);
    std::ofstream file(logFile_, std::ios::app);
    json entry = {{"timestamp", getUTCTimeStamp()},
                  {"operation", operation},
                  {"key", key},
                  {"value", value}};
    file << entry.dump() << std::endl;
    file.flush();
}

}  // namespace storage
