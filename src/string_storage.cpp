#include "string_storage.h"

#include "wal_logger.h"

#include <memory>
#include <nlohmann/json.hpp>

namespace storage {
StringStorage::StringStorage(const std::string& logFile)
    : operation_logger_(std::make_unique<WALLogger>(logFile)) {}

bool StringStorage::put(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    operation_logger_->logOperation("PUT", key, value);
    storage_[key] = value;
    return true;
}

bool StringStorage::remove(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    if (storage_.find(key) != storage_.end()) {
        storage_.erase(key);
        operation_logger_->logOperation("DELETE", key, "");
        return true;
    }
    return false;
}

std::string StringStorage::get(const std::string& key) {
    std::shared_lock<std::shared_mutex> lock_guard(mtx_);
    if (storage_.find(key) != storage_.end()) {
        return storage_[key];
    }
    return "";
}
}  // namespace storage
