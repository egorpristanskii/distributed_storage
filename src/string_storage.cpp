#include "string_storage.h"

namespace storage {
bool StringStorage::put(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    storage_[key] = value;
    return true;
}

bool StringStorage::remove(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    if (storage_.find(key) != storage_.end()) {
        storage_.erase(key);
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
