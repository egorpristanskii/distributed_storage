#pragma once

#include "value.h"
#include "wal_logger.h"

#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>

namespace storage {
class Storage {
   public:
    explicit Storage(const std::string& logFile);

    bool put(const std::string& key, ValuePtr value);
    bool remove(const std::string& key);
    std::unique_ptr<Value> get(const std::string& key);
    json listAllData() const;

   protected:
    std::unordered_map<std::string, ValuePtr> storage_;
    std::unique_ptr<WALLogger> operation_logger_;
    std::shared_mutex mtx_;
};
}  // namespace storage
