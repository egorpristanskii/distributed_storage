#include "storage.h"

#include "value.h"

#include <nlohmann/json.hpp>

namespace storage {
Storage::Storage(const std::string& logFile)
    : operation_logger_(std::make_unique<WALLogger>(logFile)) {}

bool Storage::put(const std::string& key, ValuePtr value) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    operation_logger_->logOperation("PUT", key, value->toString());
    storage_[key] = std::move(value);
    return true;
}

bool Storage::remove(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    if (storage_.find(key) != storage_.end()) {
        storage_.erase(key);
        operation_logger_->logOperation("DELETE", key, "");
        return true;
    }
    return false;
}

ValuePtr Storage::get(const std::string& key) {
    std::shared_lock<std::shared_mutex> lock_guard(mtx_);
    if (storage_.find(key) != storage_.end()) {
        return storage_[key]->cloneData();
    }
    return nullptr;
}

// void Storage::delegate() {
//     auto self = this->shared_from_this();
//     std::cout << "Delegate string storage" << std::endl;
// }

}  // namespace storage
