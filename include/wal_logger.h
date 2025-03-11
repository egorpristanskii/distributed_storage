#pragma once

#include <mutex>
#include <nlohmann/json.hpp>
#include <string>

namespace storage {
using json = nlohmann::json;

class WALLogger {
   public:
    explicit WALLogger(const std::string& logFile);
    void logOperation(const std::string& operation, const std::string& key,
                      const std::string& value,
                      const std::string_view& typeName);

    // void recoverFromLog();

   protected:
    std::string logFile_;
    std::mutex mtx_;
};
}  // namespace storage
