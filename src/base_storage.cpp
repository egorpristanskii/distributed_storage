#include "base_storage.h"
#include <mutex>

namespace storage {
bool BaseStorage::put(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock_guard(mtx_);
    storage_[key] = value;
    return true;
}

bool BaseStorage::remove(const std::string& key) {
    std::lock_guard<std::mutex> lock_guard(mtx_);
    if (storage_.find(key) != storage_.end()) {
        storage_.erase(key);
        return true;
    }
    return false;
}

std::string BaseStorage::get(const std::string& key) {
    std::lock_guard<std::mutex> lock_guard(mtx_);
    if (storage_.find(key) != storage_.end()) {
        return storage_[key];
    }
    return "";
}
}  // namespace storage
