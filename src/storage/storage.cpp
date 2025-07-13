#include "storage/storage.h"

#include "logger/logger.h"
#include "storage/types.h"
#include "storage/wal_logger.h"

#include <cstddef>
#include <nlohmann/json.hpp>

namespace storage {
Storage::Storage(std::shared_ptr<WALLogger> operation_logger)
    : operation_logger_(std::move(operation_logger)) {
    auto recovery_log = operation_logger_->recoverFromLog();
    for (const auto& [key, typed_value] : recovery_log) {
        auto type_it = kStringToType.find(typed_value.first);
        if (type_it != kStringToType.end()) {
            switch (type_it->second) {
                case TypeNameEnum::StringType:
                    storage_[key] =
                        std::make_unique<StringData>(typed_value.second);
                    break;
                case TypeNameEnum::IntType:
                    storage_[key] =
                        std::make_unique<IntData>(typed_value.second);
                    break;
                default:
                    break;
            }
        }
    }
    for (const auto& [key, value] : storage_) {
        LOG_DEBUG("Key {} with value {}", key, value->toString());
    }
}

std::unique_ptr<Value> Storage::put(const std::string& key, ValuePtr value) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    operation_logger_->logOperation("PUT", key, value->toString(),
                                    value->typeName());
    storage_[key] = std::move(value);
    return storage_[key]->cloneData();
}

bool Storage::remove(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    if (storage_.find(key) != storage_.end()) {
        storage_.erase(key);
        operation_logger_->logOperation("DELETE", key, "", "");
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

json Storage::listAllData() {
    std::shared_lock<std::shared_mutex> lock_guard(mtx_);
    json response;
    for (const auto& [key, value] : storage_) {
        response[key] = value->mapView();
    }
    return response;
}
}  // namespace storage
