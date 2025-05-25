#include "network/request.h"

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
    ASSERT_STREQ(request.method.c_str(), "POST");
    ASSERT_STREQ(request.path.c_str(), "get");
    ASSERT_STREQ(request.headers["Content-Length"].c_str(), "58");
    ASSERT_STREQ(request.headers["Content-Type"].c_str(), "application/json");
    ASSERT_STREQ(request.body.c_str(),
                 "{\"key\": \"testKey\", \"value\": \"testValue\", \"type\": "
                 "\"string\"}");
}
