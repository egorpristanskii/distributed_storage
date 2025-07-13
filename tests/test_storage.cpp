#include "storage/storage.h"

#include <filesystem>
#include <gtest/gtest.h>

using storage::Storage;

namespace {
using storage::IntData;
using storage::Storage;
using storage::StringData;

class StorageTest : public ::testing::Test {
   protected:
    void SetUp() override {
        // Remove log file if it exists to start with a clean slate.
        std::filesystem::remove(kTestLogFile_);
    }

    void TearDown() override {
        // Clean up the log file after tests.
        std::filesystem::remove(kTestLogFile_);
    }

    const std::string kTestLogFile_ = "test_wal.log";
};

TEST_F(StorageTest, PutAndGet) {
    auto operation_logger = std::make_shared<storage::WALLogger>(kTestLogFile_);
    Storage storage(operation_logger);

    storage.put("key1", std::make_unique<StringData>("value1"));
    storage.put("key2", std::make_unique<IntData>("42"));

    auto value1 = storage.get("key1");
    ASSERT_NE(value1, nullptr);
    EXPECT_EQ(value1->toString(), "value1");

    auto value2 = storage.get("key2");
    ASSERT_NE(value2, nullptr);
    EXPECT_EQ(value2->toString(), "42");
}

TEST_F(StorageTest, Remove) {
    auto operation_logger = std::make_shared<storage::WALLogger>(kTestLogFile_);
    Storage storage(operation_logger);

    storage.put("key1", std::make_unique<StringData>("value1"));
    storage.remove("key1");

    auto value = storage.get("key1");
    EXPECT_EQ(value, nullptr);
}

TEST_F(StorageTest, RecoveryFromLog) {
    {
        auto operation_logger =
            std::make_shared<storage::WALLogger>(kTestLogFile_);
        Storage storage(operation_logger);
        storage.put("key1", std::make_unique<StringData>("value1"));
        storage.put("key2", std::make_unique<IntData>("42"));
    }
    auto operation_logger = std::make_shared<storage::WALLogger>(kTestLogFile_);
    Storage recovered_storage(operation_logger);

    auto value1 = recovered_storage.get("key1");
    ASSERT_NE(value1, nullptr);
    EXPECT_EQ(value1->toString(), "value1");

    auto value2 = recovered_storage.get("key2");
    ASSERT_NE(value2, nullptr);
    EXPECT_EQ(value2->toString(), "42");
}

TEST_F(StorageTest, listAllData) {
    auto operation_logger = std::make_shared<storage::WALLogger>(kTestLogFile_);
    Storage storage(operation_logger);
    storage.put("key1", std::make_unique<StringData>("value1"));
    storage.put("key2", std::make_unique<IntData>("42"));

    storage::json data = storage.listAllData();

    ASSERT_TRUE(data.contains("key1"));
    ASSERT_TRUE(data.contains("key2"));
    ASSERT_EQ(data.size(), 2);
    ASSERT_EQ(data["key1"].size(), 2);
    ASSERT_EQ(data["key2"].size(), 2);
    EXPECT_EQ(data["key1"]["type"], "std::string");
    EXPECT_EQ(data["key1"]["value"], "value1");
    ASSERT_EQ(data["key1"].size(), 2);
    EXPECT_EQ(data["key2"]["type"], "int");
    EXPECT_EQ(data["key2"]["value"], "42");
}

}  // namespace
