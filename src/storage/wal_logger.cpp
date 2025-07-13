#include "storage/wal_logger.h"

#include "logger/logger.h"
#include "storage/utils.h"

#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>

namespace storage {
WALLogger::WALLogger(const std::string& logFile)
    : logFile_(std::move(logFile)) {
    std::ofstream file(logFile_, std::ios::app);
}

WALLogger::~WALLogger() {
    flushLog();
}

void WALLogger::logOperation(const std::string& operation,
                             const std::string& key, const std::string& value,
                             const std::string_view& typeName) {
    std::unique_lock<std::mutex> lock_guard(mtx_);
    log_queue_.emplace(json{{"timestamp", getUTCTimeStamp()},
                            {"operation", operation},
                            {"key", key},
                            {"value", value},
                            {"typename", typeName}});
}

void WALLogger::flushLog() {
    std::unique_lock<std::mutex> lock_guard(mtx_);
    std::ofstream file(logFile_, std::ios::app);
    while (!log_queue_.empty()) {
        auto entry = log_queue_.front();
        log_queue_.pop();
        file << entry.dump() << std::endl;
    }
    file.flush();
}

// TODO (e-pristanskii) move key to another file and error handling
void WALLogger::processLog(LogOperation& operationMap, const json& logJson) {
    std::string operation = logJson["operation"];
    std::string value = logJson["value"];
    std::string key = logJson["key"];
    std::string value_type = logJson["typename"];

    auto operation_it = kStringToOperation.find(operation);

    switch (operation_it->second) {
        case OperationEnum::Put:
            operationMap[key] = {value_type, value};
            break;
        case OperationEnum::Delete:
            if (operationMap.find(key) != operationMap.end())
                operationMap.erase(key);
            break;
        default:
            throw std::invalid_argument("Unknown operation");
            break;
    }
}

LogOperation WALLogger::recoverFromLog() {
    std::ifstream file(logFile_);
    LogOperation recovered_storage;

    std::string log_line;
    while (std::getline(file, log_line)) {
        try {
            json log_entry = json::parse(log_line);
            processLog(recovered_storage, log_entry);
        } catch (const json::parse_error& error) {
            LOG_ERROR("Skipping invalid log line with error", error.what());
        }
    }
    return recovered_storage;
}

}  // namespace storage
