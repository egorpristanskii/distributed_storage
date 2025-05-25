#include "app.h"

#include <filesystem>
#include <gtest/gtest.h>

class AppTest : public ::testing::Test {
   protected:
    void SetUp() override {
        std::filesystem::path source_file = __FILE__;
        std::filesystem::path log_file =
            source_file.parent_path() / "test_data" / "test_storage.log";
        std::filesystem::path copy_log_file =
            source_file.parent_path() / "test_data" / "test_storage_copy.log";
        std::filesystem::copy_file(
            log_file, copy_log_file,
            std::filesystem::copy_options::overwrite_existing);
        app_ = std::make_shared<app::Application>(copy_log_file);
    }

    void TearDown() override {
        std::filesystem::path source_file = __FILE__;
        std::filesystem::path copy_log_file =
            source_file.parent_path() / "test_data" / "test_storage_copy.log";
        if (std::filesystem::exists(copy_log_file)) {
            std::filesystem::remove(copy_log_file);
        }
    }

    std::shared_ptr<app::Application> app_;
};

TEST_F(AppTest, TestListAllData) {
    auto response = app_->handleRequest("allkeys", nlohmann::json{});
    ASSERT_EQ(response.status_code, 200);
    ASSERT_STREQ(
        response.response_data.c_str(),
        "{\"new_int\":{\"type\":\"int\",\"value\":\"122\"},"
        "\"testKey\":{\"type\":\"std::string\",\"value\":\"testValue22\"},"
        "\"testKey22\":{\"type\":\"std::string\",\"value\":\"testValue22333\"},"
        "\"testKey2233\":{\"type\":\"std::string\",\"value\":\"updated\"}}");
}

TEST_F(AppTest, TestGet) {
    nlohmann::json request = {{"key", "testKey"}};
    auto response = app_->handleRequest("get", request);
    ASSERT_EQ(response.status_code, 200);
    ASSERT_STREQ(response.response_data.c_str(), "testValue22");
    ASSERT_STREQ(response.toString().c_str(),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 11\r\n\r\n"
                 "testValue22");
}

TEST_F(AppTest, TestPut) {
    nlohmann::json request = {
        {"key", "testaddkey"}, {"value", "updated"}, {"type", "string"}};
    auto response = app_->handleRequest("put", request);
    auto get_response =
        app_->handleRequest("get", nlohmann::json{{"key", "testaddkey"}});

    ASSERT_EQ(get_response.status_code, 200);
    ASSERT_STREQ(get_response.response_data.c_str(), "updated");
    ASSERT_STREQ(get_response.toString().c_str(),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 7\r\n\r\n"
                 "updated");
    ASSERT_EQ(response.status_code, 200);
    ASSERT_STREQ(response.response_data.c_str(), "success");
    ASSERT_STREQ(response.toString().c_str(),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 7\r\n\r\n"
                 "success");
}

TEST_F(AppTest, TestRemove) {
    nlohmann::json request = {{"key", "testKey22"}};
    auto response = app_->handleRequest("remove", request);
    auto get_response =
        app_->handleRequest("get", nlohmann::json{{"key", "testKey22"}});
    ASSERT_EQ(get_response.status_code, 400);
    ASSERT_STREQ(get_response.response_data.c_str(), "");
    ASSERT_STREQ(get_response.toString().c_str(),
                 "HTTP/1.1 400 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 0\r\n\r\n");
    ASSERT_EQ(response.status_code, 200);
    ASSERT_STREQ(response.response_data.c_str(), "success");
    ASSERT_STREQ(response.toString().c_str(),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 7\r\n\r\n"
                 "success");
}
