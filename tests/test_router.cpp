#include "network/response.h"
#include "network/router.h"

#include <gtest/gtest.h>

namespace {
constexpr char kTestInput[] = R"({"test":"empty"})";
}

class TestHandler {
   public:
    network::Response ping(const nlohmann::json&) {  // NOLINT
        return network::Response(222, "pong");
    }
};

TEST(TestRouter, TestPing) {
    auto test_router = router::Router();
    auto test_handler = TestHandler();
    test_router.addRoute("ping", test_handler, &TestHandler::ping);
    network::Response result =
        test_router.handleRoute("ping", nlohmann::json::parse(kTestInput));
    ASSERT_EQ(result.status_code, 222);
    ASSERT_STREQ(result.response_data.c_str(), "pong");
    ASSERT_STREQ(result.toString().c_str(),
                 "HTTP/1.1 222 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: 4\r\n\r\n"
                 "pong");
}
