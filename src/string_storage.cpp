#include "string_storage.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace storage {
bool StringStorage::put(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock_guard(mtx_);
    if (!std::filesystem::exists("/tmp/data.json")) {
        std::ofstream file("/tmp/data.json", std::ios::out);
        file << "[]";
    }

    std::ifstream in_file("/tmp/data.json");
    nlohmann::json wal;
    in_file >> wal;
    in_file.close();
    nlohmann::json entry = {
        {"key", key}, {"value", value}, {"operation", "PUT"}};
    wal.push_back(entry);
    std::ofstream out_file("/tmp/data.json", std::ios::trunc);
    out_file << wal.dump(4);
    out_file.flush();
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
