#pragma once

#include "types.h"

#include <mutex>
#include <nlohmann/json.hpp>
#include <queue>
#include <string>

namespace storage {
using json = nlohmann::json;

class WALLogger {
   public:
    explicit WALLogger(const std::string& logFile);
    ~WALLogger();
    void logOperation(const std::string& operation, const std::string& key,
                      const std::string& value,
                      const std::string_view& typeName);

    void flushLog();

    LogOperation recoverFromLog();

   protected:
    static void processLog(LogOperation& operationMap, const json& logJson);

    std::string logFile_;
    std::mutex mtx_;
    std::queue<nlohmann::json> log_queue_;
};
}  // namespace storage
