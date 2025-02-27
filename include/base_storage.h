#pragma once

#include <mutex>
#include <unordered_map>

namespace storage {
class BaseStorage {
   public:
    bool put(const std::string& key, const std::string& value);
    bool remove(const std::string& key);
    std::string get(const std::string& key);

   protected:
    std::unordered_map<std::string, std::string> storage_;
    std::mutex mtx_;
};
}  // namespace storage
