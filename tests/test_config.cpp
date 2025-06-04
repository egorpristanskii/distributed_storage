#include "config.h"

#include <filesystem>
#include <gtest/gtest.h>

TEST(TestBuildConfig, TestBuildConfig) {
    std::filesystem::path source_file = __FILE__;
    std::filesystem::path log_file =
        source_file.parent_path() / "test_data" / "test_config.json";

    app::Config config = app::buildConfig(log_file.string());
    ASSERT_EQ(config.logFile, "test_log.log");
    ASSERT_EQ(config.storageFile, "test_storage.log");
    ASSERT_EQ(config.numThreads, 4);
    ASSERT_EQ(config.port, 8080);
}
