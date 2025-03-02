#pragma once

#include "storage.h"
#include "wal_logger.h"

#include <memory>
#include <shared_mutex>

namespace storage {
class StringStorage : public Storage<std::string> {
   public:
    explicit StringStorage(const std::string& logFile);
    bool put(const std::string& key, const std::string& value) override;
    bool remove(const std::string& key) override;
    std::string get(const std::string& key) override;
    void recoverFromLog();

    ~StringStorage() override = default;

   protected:
    std::shared_mutex mtx_;
    std::unique_ptr<WALLogger> operation_logger_;
};
}  // namespace storage
