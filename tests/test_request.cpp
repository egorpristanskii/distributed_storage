#include "network/request.h"
#include "network/types.h"

#include <gtest/gtest.h>

TEST(TestHTTRequest, TestInitFromStream) {
    std::string raw_request =
        "POST /get HTTP/1.1\r\n"
        "Host: localhost:8081\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 58\r\n"
        "\r\n"
        "{\"key\": \"testKey\", \"value\": \"testValue\", \"type\": "
        "\"string\"}";

    network::Request request{raw_request};
    ASSERT_EQ(request.method, network::HTTPMethod::POST);
    ASSERT_STREQ(request.path.c_str(), "get");
    ASSERT_STREQ(request.headers["Content-Length"].c_str(), "58");
    ASSERT_STREQ(request.headers["Content-Type"].c_str(), "application/json");
    ASSERT_STREQ(request.body.c_str(),
                 "{\"key\": \"testKey\", \"value\": \"testValue\", \"type\": "
                 "\"string\"}");
}

TEST(TestHTTRequest, TestHTTPMethodConvert) {
    std::string post_raw_request =
        "POST /get HTTP/1.1\r\n"
        "Host: localhost:8081\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 58\r\n"
        "\r\n"
        "{\"key\": \"testKey\", \"value\": \"testValue\", \"type\": "
        "\"string\"}";

    network::Request post_request{post_raw_request};
    std::string get_raw_request =
        "GET /get HTTP/1.1\r\n"
        "Host: localhost:8081\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 58\r\n"
        "\r\n"
        "{\"key\": \"testKey\", \"value\": \"testValue\", \"type\": "
        "\"string\"}";
    network::Request get_request{get_raw_request};
    std::string delete_raw_request =
        "DELETE /get HTTP/1.1\r\n"
        "Host: localhost:8081\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 58\r\n"
        "\r\n"
        "{\"key\": \"testKey\", \"value\": \"testValue\", \"type\": "
        "\"string\"}";
    network::Request delete_request{delete_raw_request};
    std::string put_raw_request =
        "PUT /get HTTP/1.1\r\n"
        "Host: localhost:8081\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 58\r\n"
        "\r\n"
        "{\"key\": \"testKey\", \"value\": \"testValue\", \"type\": "
        "\"string\"}";
    network::Request put_request{put_raw_request};

    ASSERT_EQ("POST", post_request.methodToString());
    ASSERT_EQ("GET", get_request.methodToString());
    ASSERT_EQ("DELETE", delete_request.methodToString());
    ASSERT_EQ("PUT", put_request.methodToString());
}
