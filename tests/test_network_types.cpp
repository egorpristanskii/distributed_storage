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

    network::HTTPRequest request{raw_request};
    std::cerr << "Method: " << request.method << std::endl;
    std::cerr << "Path: " << request.path << std::endl;
    std::cerr << "Body: " << request.body << std::endl;
    std::cerr << "Headers " << request.headers["Content-Length"] << std::endl;
    ASSERT_STREQ(request.method.c_str(), "POST");
    ASSERT_STREQ(request.path.c_str(), "/get");
    ASSERT_STREQ(request.headers["Content-Length"].c_str(), "58");
    ASSERT_STREQ(request.headers["Content-Type"].c_str(), "application/json");
    ASSERT_STREQ(request.body.c_str(),
                 "{\"key\": \"testKey\", \"value\": \"testValue\", \"type\": "
                 "\"string\"}");
}
