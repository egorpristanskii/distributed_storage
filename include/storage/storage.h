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
    explicit Storage(std::shared_ptr<WALLogger> operation_logger);

    std::unique_ptr<Value> put(const std::string& key, ValuePtr value);
    bool remove(const std::string& key);
    std::unique_ptr<Value> get(const std::string& key);
    json listAllData();

   protected:
    std::unordered_map<std::string, ValuePtr> storage_;
    std::shared_ptr<WALLogger> operation_logger_;
    std::shared_mutex mtx_;
};
}  // namespace storage
