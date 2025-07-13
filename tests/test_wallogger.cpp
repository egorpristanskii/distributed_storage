#include "storage/wal_logger.h"

#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>

namespace {
using storage::json;

class WALLoggerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        log_file_ = "test_wal.log";
        logger_ = std::make_unique<storage::WALLogger>(log_file_);
    }

    void TearDown() override { std::remove(log_file_.c_str()); }

    std::unique_ptr<storage::WALLogger> logger_;
    std::string log_file_;
};

TEST_F(WALLoggerTest, LogOperationPut) {
    logger_->logOperation("PUT", "key1", "value1", "std::string");
    logger_->flushLog();

    std::ifstream file(log_file_);
    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line);

    json entry = json::parse(line);
    ASSERT_EQ(entry["operation"], "PUT");
    ASSERT_EQ(entry["key"], "key1");
    ASSERT_EQ(entry["value"], "value1");
    ASSERT_EQ(entry["typename"], "std::string");
    ASSERT_TRUE(entry.contains("timestamp"));
}

TEST_F(WALLoggerTest, LogOperationDelete) {
    logger_->logOperation("DELETE", "key1", "", "std::string");
    logger_->flushLog();

    std::ifstream file(log_file_);
    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line);

    json entry = json::parse(line);
    ASSERT_EQ(entry["operation"], "DELETE");
    ASSERT_EQ(entry["key"], "key1");
    ASSERT_EQ(entry["value"], "");
    ASSERT_EQ(entry["typename"], "std::string");
    ASSERT_TRUE(entry.contains("timestamp"));
}

TEST_F(WALLoggerTest, RecoverFromLogPutOperation) {
    logger_->logOperation("PUT", "key1", "value1", "std::string");
    logger_->logOperation("PUT", "key2", "value2", "std::string");
    logger_->flushLog();

    storage::LogOperation recovered_storage = logger_->recoverFromLog();

    ASSERT_EQ(recovered_storage.size(), 2);
    ASSERT_EQ(recovered_storage["key1"].first, "std::string");
    ASSERT_EQ(recovered_storage["key1"].second, "value1");
    ASSERT_EQ(recovered_storage["key2"].first, "std::string");
    ASSERT_EQ(recovered_storage["key2"].second, "value2");
}

TEST_F(WALLoggerTest, RecoverFromLogDeleteOperation) {
    logger_->logOperation("PUT", "key1", "value1", "std::string");
    logger_->logOperation("DELETE", "key1", "", "std::string");
    logger_->flushLog();

    storage::LogOperation recovered_storage = logger_->recoverFromLog();

    ASSERT_TRUE(recovered_storage.empty());
}

TEST_F(WALLoggerTest, RecoverFromLogMixedOperations) {
    logger_->logOperation("PUT", "key1", "value1", "std::string");
    logger_->logOperation("PUT", "key2", "value2", "std::string");
    logger_->logOperation("DELETE", "key1", "", "std::string");
    logger_->flushLog();

    storage::LogOperation recovered_storage = logger_->recoverFromLog();

    ASSERT_EQ(recovered_storage.size(), 1);
    ASSERT_EQ(recovered_storage["key2"].first, "std::string");
    ASSERT_EQ(recovered_storage["key2"].second, "value2");
}

TEST_F(WALLoggerTest, RecoverFromLogInvalidJson) {
    std::ofstream file(log_file_, std::ios::app);
    file << "Invalid JSON Line\n";
    file.close();

    storage::LogOperation recovered_storage = logger_->recoverFromLog();

    ASSERT_TRUE(recovered_storage.empty());
}
}  // namespace
