#include "app.h"
#include "network/http_codes.h"
#include "network/types.h"

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
    auto response = app_->handleRequest(
        {network::HTTPMethod::GET, "allkeys", nlohmann::json{}.dump()});
    ASSERT_EQ(response.status_code, network::HTTPCode::OK);
    ASSERT_STREQ(
        response.response_data.c_str(),
        "{\"new_int\":{\"type\":\"int\",\"value\":\"122\"},"
        "\"testKey\":{\"type\":\"std::string\",\"value\":\"testValue22\"},"
        "\"testKey22\":{\"type\":\"std::string\",\"value\":\"testValue22333\"},"
        "\"testKey2233\":{\"type\":\"std::string\",\"value\":\"updated\"}}");
}

TEST_F(AppTest, TestGet) {
    nlohmann::json request;
    auto response = app_->handleRequest(
        {network::HTTPMethod::GET, "keys/testKey", request.dump()});
    ASSERT_EQ(response.status_code, network::HTTPCode::OK);
    ASSERT_STREQ(response.response_data.c_str(),
                 "{\"key\":\"testKey\",\"type\":\"std::string\",\"value\":"
                 "\"testValue22\"}");
    ASSERT_STREQ(response.toString().c_str(),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 60\r\n\r\n"
                 "{\"key\":\"testKey\",\"type\":\"std::string\",\"value\":"
                 "\"testValue22\"}");
}

TEST_F(AppTest, TestPut) {
    nlohmann::json request = {{"value", "updated"}, {"type", "string"}};
    auto response = app_->handleRequest(
        {network::HTTPMethod::POST, "keys/testaddkey", request.dump()});
    auto get_response = app_->handleRequest(
        {network::HTTPMethod::GET, "keys/testaddkey", nlohmann::json{}.dump()});

    ASSERT_EQ(get_response.status_code, network::HTTPCode::OK);
    ASSERT_STREQ(get_response.response_data.c_str(),
                 "{\"key\":\"testaddkey\",\"type\":\"std::string\",\"value\":"
                 "\"updated\"}");
    ASSERT_STREQ(get_response.toString().c_str(),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 59\r\n\r\n"
                 "{\"key\":\"testaddkey\",\"type\":\"std::string\",\"value\":"
                 "\"updated\"}");
    ASSERT_EQ(response.status_code, network::HTTPCode::OK);
    ASSERT_STREQ(response.response_data.c_str(),
                 "{\"key\":\"testaddkey\",\"type\":\"std::string\",\"value\":"
                 "\"updated\"}");
    ASSERT_STREQ(response.toString().c_str(),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 59\r\n\r\n"
                 "{\"key\":\"testaddkey\",\"type\":\"std::string\",\"value\":"
                 "\"updated\"}");
}

TEST_F(AppTest, TestRemove) {
    nlohmann::json request;
    auto response = app_->handleRequest(
        {network::HTTPMethod::DELETE, "keys/testKey22", request.dump()});
    auto get_response = app_->handleRequest(
        {network::HTTPMethod::GET, "keys/testKey22", nlohmann::json{}.dump()});
    ASSERT_EQ(get_response.status_code, network::HTTPCode::NotFound);
    ASSERT_STREQ(get_response.response_data.c_str(),
                 "{\"error\":\"Key not found\"}");
    ASSERT_STREQ(get_response.toString().c_str(),
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 25\r\n\r\n"
                 "{\"error\":\"Key not found\"}");
    ASSERT_EQ(response.status_code, network::HTTPCode::NoContent);
    ASSERT_STREQ(response.response_data.c_str(), "");
    ASSERT_STREQ(response.toString().c_str(),
                 "HTTP/1.1 204 No Content\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 0\r\n\r\n");
}
