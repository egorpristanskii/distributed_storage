#pragma once

#include "storage.h"

#include <shared_mutex>
#include <unordered_map>

namespace storage {
class StringStorage : public Storage {
   public:
    bool put(const std::string& key, const std::string& value) override;
    bool remove(const std::string& key) override;
    std::string get(const std::string& key) override;

    ~StringStorage() override = default;

   protected:
    std::unordered_map<std::string, std::string> storage_;
    std::shared_mutex mtx_;
};
}  // namespace storage
